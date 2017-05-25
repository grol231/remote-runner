#ifndef __Service_H__
#define __Service_H__

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/error.hpp>
#include "log.h"
#include "config.h"

class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock,     
        boost::asio::io_service& ios,
        unsigned long long int connect_id,
        src::severity_logger<logging::trivial::severity_level>& log) :
            sock_(sock),
            buffer_(std::make_shared<boost::asio::streambuf>()),                   
            ios_(ios),
            timer_(ios_),
            statistic_(),
            log_(log)          
    {
        std::cout << "Service created." << std::endl;
        statistic_.ConnectID = connect_id;
    }
    ~Service(){
        std::cout << "Service destroyed!" << std::endl;
        BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(statistic_);
    }
    void StartHandling(std::shared_ptr<Config> config)
    {
       std::cout << "StartHandling" << std::endl;       
       std::weak_ptr<Service> weakSelf = shared_from_this();
       handling_ = [weakSelf, config](){
           std::cout << "lambda: handling_" << std::endl;
           std::shared_ptr<Service> self = weakSelf.lock();
           boost::asio::async_read_until(*self->sock_.get(),
               *self->buffer_.get(),
               '\n',
               [self, config](const boost::system::error_code& ec,
                      std::size_t bytes_transffered)
              { 
                     if(0 == ec)
                     {
                         if(1 < bytes_transffered)//This is crutch!
                         {                              
                             std::cout<<"Async operation success!"<<std::endl;
                             self->OnRequestReceived(bytes_transffered, config);
                         }
                         self->buffer_->consume(bytes_transffered);
                         std::cout << "handling_ call himself!" << std::endl;
                         self->handling_();
                     }
                     else
                     {
                        std::cout << "Async operation error!" << std::endl;
                     }
              }
           );
       };
       handling_();
    }
private:
    void OnRequestReceived(std::size_t bytes_transferred,
            std::shared_ptr<Config> config)
    {
        std::cout << "onRequestReceived" << std::endl;
        response_ = ProcessRequest(buffer_, bytes_transferred, config);
        boost::asio::async_write(*sock_.get(),
            boost::asio::buffer(response_),
            [this](const boost::system::error_code& ec,
                std::size_t bytes_transferred)
            {
                onResponseSent(ec, bytes_transferred);
            }
        );
    }
    void onResponseSent(const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        std::cout << "onResponseSent" << std::endl;
        if (ec != 0)
        {
            std::cout << "Error occured! Error code = "
                << ec.value()
                << ". Message: " << ec.message();
        }
    }

    std::string ProcessRequest(std::shared_ptr<boost::asio::streambuf> buffer,
            std::size_t bytes_transferred,
            std::shared_ptr<Config> config)
    {
        std::cout << "ProcessRequest" << std::endl;
        std::string command;
        std::istream(buffer.get()) >> command;
        std::string response = "Response";
        std::cout << "Request:" << command << std::endl;
        Logging::LogRecord record;
        record.Command = command;
        record.Condition = "start";
        
        auto list = config->AllowCommands();
        if(!list.empty() &&
            list.end() 
                == 
            std::find(list.begin(), list.end(), command))
        {
            std::string message = " - not allow command!";
            std::cout << message << std::endl;
            record.Result = "fail";
            record.Note = message; 
            ++statistic_.NotRunningCommandCounter;
            BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
            return response + message + "\n";
        }
        pid_t pid = fork();//todo: error handling
        int err(0);
        if(pid < 0)
        {
            std::cout << "fork fail!" << std::endl;
            response += ":fork fail - ";
            std::string message = ProcessError(errno);
            response += message;
            record.Result = "fail";
            record.Note = message; 
            ++statistic_.NotRunningCommandCounter;
            BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
            return response;
        }
        if(!pid)
        {
            std::cout << "child:" << pid  << std::endl;
            execlp(command.c_str(),command.c_str(),NULL);
            //execlp("gedit","gedit","mylog.txt");
            //execlp("vlc","vlc","video.mp4");
            perror("child");
            exit(1);
        }
        else
        {
            
            ++statistic_.RunningCommandCounter;
            record.Result = "success";
            size_t num = timer_.expires_from_now(config->Timeout());
            if(0!=num)
                std::cout << "Too late! Timer already expires!" <<  std::endl;
            timer_.async_wait([this,pid](const boost::system::error_code& ec){
                unsigned int result = kill(pid,SIGKILL);
                if(result == 0)                
                    ++statistic_.CompletedCompulsorilyCommandCounter;                
                else                
                    ++statistic_.CompletedCommandCounter;                
                std::cout << "Kill child process! pId:" << pid <<  std::endl;
            });
            
            response += "Success execution!";
        }            
        response += "\n";
        BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
        return response;
    }
    std::shared_ptr<boost::asio::ip::tcp::socket> Socket(){return sock_;}
    std::shared_ptr<boost::asio::streambuf> Buffer(){return buffer_;}
private:
    std::string ProcessError(int err)
    {
        std::string result;
        switch(err)
        {
            case EAGAIN:
                result = "The system lacked the necessary resources to create another process.";
            break;
            case ENOMEM:
                result = "Insufficient storage is available.";
            break;
            default:
                result += "Uknown error code.";
        }
        return result;
    }

    std::string response_;
    boost::asio::io_service& ios_;
    boost::asio::deadline_timer timer_;
    Logging::Statistic statistic_;
    src::severity_logger<logging::trivial::severity_level>& log_;
    std::function<void(void)> handling_;
    std::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    std::shared_ptr<boost::asio::streambuf> buffer_;
};
#endif
