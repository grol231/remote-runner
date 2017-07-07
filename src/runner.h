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
#include "registrar.h"

class Registrar;
class Runner 
{
public:
    Runner(boost::asio::io_service& ios, 
           boost::posix_time::seconds timeout, 
           std::shared_ptr<Registrar> registrar):
        timeout_(timeout),
        timer_(ios),
        registrar_(registrar)
    {
        std::cout << "Runner created!" << std::endl;
    }
    ~Runner()
    {
        std::cout << "Runner destoryed!" << std::endl;
    }    
    Runner& operator=(const Runner&) = delete;
    Runner(const Runner&) = delete;
    Runner&& operator=(Runner&&) = delete;
    Runner(Runner&&) = delete;

    void Execute(std::string& command, std::vector<std::string>& args)
    {
        std::cout << "Runner::Execute" << std::endl;
        pid_t pid = fork();
        int err(0);
        if(pid < 0)
        {
            std::string error = ProcessError(errno);
            registrar_->RegisterFailedLaunch(command, args, error);
            std::cout << "Failed fork!" << std::endl;
        }
        if(!pid)
        {
            std::cout << "Child process" << std::endl;
            DoExecute(command, args);
        }
        else
        {    
            std::cout << "Parent process go to Kill!" << std::endl;
           // registrar_->RegisterLaunch(command, args);        
            Kill(pid);
        }                  
    }
    std::string ProcessError(int err)
    {
        std::cout << "Runner::ProcessError" << std::endl;
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
    void Kill(pid_t pid)
    {   
        std::cout << "Runner::Kill - pid = " << pid << std::endl; 
        std::string record;    
        size_t num = timer_.expires_from_now(timeout_);
            if(0!=num)
                std::cout << "Too late! Timer already expires!" <<  std::endl;
            timer_.async_wait([this,pid](const boost::system::error_code& ec){
                unsigned int result = kill(pid,SIGKILL);
               // if(result == 0)                
                 //   registrar_->RegisterForcedTermination();
              // else
                //    registrar_->RegisterTermination();            
               std::cout << "Kill child process! pId:" << pid <<  std::endl;
           }); 
       // record += "Successful launch!";//TODO: What is it?                  
    }

private:
    void DoExecute(std::string& command, std::vector<std::string>& args) 
    {
        std::cout << "Runner:DoExecute" << std::endl;
        char** argv = CreateArgv(args);
        std::cout << "command: " << command << std::endl;    
        execvp(command.c_str(),argv);
        std::cout << "Failed execute!" << std::endl;
        perror("child");
        exit(1);
    }
    char** CreateArgv(const std::vector<std::string>& args) const 
    {
        std::cout << "Runner::CreateArgv" << std::endl;
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
    std::shared_ptr<Registrar> registrar_;
};
#endif
