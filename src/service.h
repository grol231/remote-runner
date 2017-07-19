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
#include <boost/algorithm/string.hpp>
#include "log.h"
#include "config.h"
#include "runner.h"
#include "registrar.h"

//TODO:
//namespace rr{} //remote-runnerd

class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock,     
        boost::asio::io_service& ios,
        unsigned long long int connect_id,
        boost::posix_time::seconds timeout,
        const std::vector<std::string>& allow_commands) :
            sock_(sock),
            buffer_(std::make_shared<boost::asio::streambuf>()),                   
            ios_(ios),
           // registrar_(std::make_shared<Registrar>(connect_id)),
            runner_(std::make_shared<Runner>(ios_, timeout)),
            allow_commands_(allow_commands)
    {
        std::cout << "Service created." << std::endl;
    }
    ~Service()
    {
        std::cout << "Service destroyed!" << std::endl;
   //     LOG(registrar_->Print());
    }
    void StartHandling()
    {
       std::cout << "Service::StartHandling" << std::endl;       
       std::weak_ptr<Service> weakSelf = shared_from_this();
       handling_ = [weakSelf](){
           std::cout << "lambda: handling_" << std::endl;
           std::shared_ptr<Service> self = weakSelf.lock();
           boost::asio::async_read_until(*self->sock_.get(),
               *self->buffer_.get(),
               '\n',
               [self](const boost::system::error_code& ec,
                      std::size_t bytes_transffered)
              { 
                     if(0 == ec)
                     {
                         if(1 < bytes_transffered)
                         {                              
                             std::cout<<"Async operation success!"<<std::endl;
                             self->OnRequestReceived(bytes_transffered);
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
    void OnRequestReceived(std::size_t bytes_transferred)
    {
        std::cout << "Service::onRequestReceived" << std::endl;
        response_ = ProcessRequest(buffer_, bytes_transferred);
        
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
        std::cout << "Service::onResponseSent" << std::endl;
        if (ec != 0)
        {
            std::cout << "Error occured! Error code = "
                << ec.value()
                << ". Message: " << ec.message();
        }
    }

    std::string ProcessRequest(std::shared_ptr<boost::asio::streambuf> buffer,
            std::size_t bytes_transferred)
    {
        std::cout << "Service::ProcessRequest" << std::endl;
        std::istream is(buffer.get());
        std::vector<std::string> args;
        while(is){
            std::string cmd;
            is >> cmd;         
            if(cmd.empty())
                break;            
            args.push_back(cmd);
        }
        std::string command(args[0]);
        if(!allow_commands_.empty() &&
            allow_commands_.end() 
                == 
            std::find(allow_commands_.begin(), allow_commands_.end(), command))
        {
            std::string message = " - not allow command!";
            std::cout << message << std::endl;
            //registrar_->RegisterFailedLaunch(command,args,message);
            //TODO: What does it write in responce?
            return "Don't worry be happy!";
        }
        runner_->Execute(command, args);
       // response += "\n";
       // TODO: What does it write in responce?
        std::cout << "Parent process: Go out from Execute!"<< std::endl;
        return "Don't worry be happy!";
    }
private:    
    std::string response_;
    boost::asio::io_service& ios_;
    std::function<void(void)> handling_;
    std::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    std::shared_ptr<boost::asio::streambuf> buffer_;
    std::shared_ptr<Runner> runner_;
    std::vector<std::string> allow_commands_;
   // std::shared_ptr<Registrar> registrar_;
};
#endif
