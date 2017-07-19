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
#include "registrar.h"

class Registrar;
class Runner 
{
public:
    Runner(boost::asio::io_service& ios,           
           std::shared_ptr<Registrar> registrar):       
        timer_(ios),
        registrar_(registrar)
    {
    }
    void Execute()
    {
        pid_t pid = fork();
        if(!pid)
        {
            DoExecute();
        }
        else
        {    
            Kill(pid);
        }                  
    }
    void Kill(pid_t pid)
    {   
        boost::posix_time::seconds t(1000);
        timer_.expires_from_now(t);
        timer_.async_wait([this,pid](const boost::system::error_code& ec){
            kill(pid,SIGKILL);
        }); 
    }

private:
    void DoExecute() 
    {
        std::string command("vlc");
        std::vector<std::string> v = {"vlc"};
        execvp(command.c_str(), CreateArgv(v));
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
    boost::asio::deadline_timer timer_;
    std::shared_ptr<Registrar> registrar_;
};
#endif
