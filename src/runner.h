#ifndef __Runner_H__
#define __Runner_H__

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

class Runner 
{
public:
    Runner(boost::asio::io_service& ios, boost::posix_time::seconds timeout):
        timeout_(timeout),
        timer_(ios){}    
    std::shared_ptr<Logging::Statistic> Execute(std::string& command, std::vector<std::string>& args)
    {
        std::shared_ptr<Logging::Statistic> stat(std::make_shared<Logging::Statistic>());
        pid_t pid = fork();
        int err(0);
        if(pid < 0)
        {
            std::cout << "fork fail!" << std::endl;
            //response += ":fork fail - ";
            //std::string message = ProcessError(errno);
            //response += message;
           // record.Result = "fail";
            //record.Note = message; 
            ++stat->NotRunningCommandCounter;
            //BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
            //return response;
        }
        if(!pid)
        {
            DoExecute(command, args);
        }
        else
        {            
            ++stat->RunningCommandCounter;
            //record.Result = "success";
            Kill(pid, stat);
         }          
        return stat;
    }
    std::string ProcessError(int err)
    {
        std::string result;
        switch(err)
        {
            case EAGAIN:
                result = "the system lacked the necessary \
                    resources to create another process.";
            break;
            case ENOMEM:
                result = "insufficient storage is available.";
            break;
            default:
                result = "uknown error code.";
        }
        return result;
 
    }
    void Kill(pid_t pid,std::shared_ptr<Logging::Statistic> stat)
    {    
        std::string record;    
        size_t num = timer_.expires_from_now(timeout_);
           if(0!=num)
               std::cout << "Too late! Timer already expires!" <<  std::endl;
           timer_.async_wait([this,pid,stat](const boost::system::error_code& ec){
               unsigned int result = kill(pid,SIGKILL);
               if(result == 0)                
                   ++stat->CompletedCompulsorilyCommandCounter;                
               else                
                   ++stat->CompletedCommandCounter;                
               std::cout << "Kill child process! pId:" << pid <<  std::endl;
           }); 
        record += "Successful launch!";//TODO: What is it?                  
    }
    Runner& operator=(const Runner&) = delete;
    Runner(const Runner&) = delete;
private:
    void DoExecute(std::string& command, std::vector<std::string>& args) 
    {
            char** argv = CreateArgv(args);
            execvp(command.c_str(),argv );
           // std::string message = ProcessError(errno);
           // response += message;
           // record.Result = "fail";
           // record.Note = message;             
           // BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
            perror("child");
            exit(1);     
    }
    char** CreateArgv(const std::vector<std::string>& args) const 
    {
        char** argv = new char*[args.size() + 1];
        for (size_t i = 0; i < args.size(); ++i) {
            argv[i] = new char[args[i].length() + 1];
            strcpy(argv[i], args[i].c_str());
        }        
        argv[args.size()] = nullptr;
        return argv;
    }
    boost::posix_time::seconds timeout_;
    boost::asio::deadline_timer timer_;
};
#endif
