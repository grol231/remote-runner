#ifndef __Daemon_H__
#define __Daemon_H__

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
class Daemon
{
public:
    Daemon(const std::function<void(void)>& init)
    {
        std::cout << "Create daemon" << std::endl;
        pid_t pid = fork();
        if(pid < 0)
        {
            std::cout << "can't fork" << std::endl;            
            exit(1);
        }
        else
        {
            if(pid != 0)            
            {
                std::cout << "I am in the parent." << std::endl;
                exit(0);
            }
            else
            {
                std::cout << "I am in the child." << std::endl;
            }                
       
        }
        setsid();
        unsigned int numFiles = sysconf(_SC_OPEN_MAX);
        for(unsigned int i = 0; i < numFiles; ++i)
        {
           close(i);
        }
        init();
    }
    Daemon() = delete;
    Daemon(const Daemon&) = delete;
    Daemon& operator=(const Daemon&) = delete;
};
#endif
