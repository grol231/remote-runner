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

class Service
{
public:
 boost::asio::streambuf Request;
   
    Service(std::vector<std::string>& allow_commands,
        boost::posix_time::seconds timeout,
        boost::asio::io_service& ios,
        unsigned long long int connect_id,
        src::severity_logger<logging::trivial::severity_level>& log) :            
            m_allow_commands(allow_commands),
            m_timeout(timeout),
            m_ios(ios),
            m_t(m_ios),
            m_connect_id(connect_id),
            m_statistic(),
            m_log(log)            
    {
        std::cout << "Create service" << std::endl;
        m_statistic.ConnectID = m_connect_id;
    }
    ~Service()
    {
        std::cout << "Service destroy!" << std::endl;
        BOOST_LOG_SEV(m_log,logging::trivial::info) << Logging::ToString(m_statistic);
    }
    void StartHandling()
    {
        std::cout << "Service::StartHandling" << std::endl;
    }

    void onRequestReceived(const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        std::cout << "Service::onRequestReceived" << std::endl;
        if (ec != 0)
        {
            std::cout << "Error occured! Error code = "
                << ec.value()
                << ". Message: " << ec.message();
            //onFinish();
            return;
        }
        m_response = ProcessRequest(Request, bytes_transferred);
        /*
        boost::asio::async_write(*sock.get(),
            boost::asio::buffer(m_response),
            [this](const boost::system::error_code& ec,
                std::size_t bytes_transferred)
            {
                onResponseSent(ec, bytes_transferred);
            }
        );
        */
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
        StartHandling();
        //onFinish();
    }
    /*void onFinish()
    {
        delete this;
    }*/
    std::string ProcessRequest(boost::asio::streambuf& request,
            std::size_t bytes_transferred)
    {
        std::cout << "Service::ProcessRequest" << std::endl;
        std::string data;
        std::istream(&request) >> data;
        std::string response = "Response";
        std::cout << "Request:" << data << std::endl;
        Logging::LogRecord record;
        record.Command = data;
        record.Condition = "start";
        if(!m_allow_commands.empty() &&
            m_allow_commands.end()
            ==
            std::find(m_allow_commands.begin(), m_allow_commands.end(), data))
        {
            std::cout << "Not allow command!" << std::endl;
            return response+":not allow command!\n";
        }
        pid_t pid = fork();//todo: error handling
        int err(0);
        if(pid < 0)
        {
            response += ":fork fail!\n";
            return response;
        }
        if(!pid)
        {
            std::cout << "Execute : " << data.c_str() << std::endl;
            err = execlp(data.c_str(),NULL);
            std::cout << "Execute failed!" << std::endl;
            ++m_statistic.NotRunningCommandCounter;
            record.Result = "fail";
            record.Note = "Unsuccess execution.";
            perror("child");
            exit(1);
        }
        else
        {
            ++m_statistic.RunningCommandCounter;
            record.Result = "success";
            size_t num = m_t.expires_from_now(m_timeout);
            if(0!=num)
                std::cout << "Too late! Timer already expires!" <<  std::endl;
            m_t.async_wait([this,pid](const boost::system::error_code& ec){
                unsigned int r = kill(pid,SIGKILL);
                if(r == 0)
                {
                    ++m_statistic.CompletedCompulsorilyCommandCounter;
                }
                else
                {
                    ++m_statistic.CompletedCommandCounter;
                }
                std::cout << "Kill child process!" << std::endl;
            });
            response += "-parent:";
        }
        if(-1 == err)
        {
            switch(errno)
            {
                case E2BIG:
                    response += "E2BIG";
                    break;
                case EACCES:
                    response += "EACCES";
                    break;
                case EFAULT:
                    response += "EFAULT";
                    break;
                case EINVAL:
                    response += "EINVAL";
                    break;
                case EIO:
                    response += "EIO";
                    break;
                case EISDIR:
                    response += "EISDIR";
                    break;
                case ELIBBAD:
                    response += "ELIBBAD";
                    break;
                case ELOOP:
                    response += "ELOOP";
                    break;
                case EMFILE:
                    response += "EMFILE";
                    break;
                case ENAMETOOLONG:
                    response += "ENAMETOOLONG";
                    break;
                case ENFILE:
                    response += "ENFILE";
                    break;
                case ENOENT:
                    response += "ENOENT";
                    break;
                case ENOEXEC:
                    response += "ENOEXEC";
                    break;
                case ENOMEM:
                    response += "ENOMEM";
                    break;
                case ENOTDIR:
                    response += "ENOTDIR";
                    break;
                case EPERM:
                    response += "EPERM";
                    break;
                case ETXTBSY:
                    response += "ETXTBSY";
                    break;
                default:
                    response += "Uknown error code.";
            }
        }
        else
        {
            response += "success";
        }
        response += "\n";
        BOOST_LOG_SEV(m_log,logging::trivial::info) << Logging::ToString(record);
        return response;
    }

private:    
    std::string m_response;
    std::vector<std::string> m_allow_commands;//TODO: Use shared_ptr!
    boost::posix_time::seconds m_timeout;
    boost::asio::io_service& m_ios;
    boost::asio::deadline_timer m_t;
    unsigned long long int m_connect_id;
    Logging::Statistic m_statistic;
    src::severity_logger<logging::trivial::severity_level>& m_log;
};
#endif
