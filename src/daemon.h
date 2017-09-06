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
        pid_t pid = fork();
        if(pid < 0)
        {
            exit(1);
        }
        else
        {
            if(pid != 0)            
            {
                exit(0);
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
    ~Daemon(){}
    Daemon() = delete;
    Daemon(const Daemon&) = delete;
    Daemon& operator=(const Daemon&) = delete;
    Daemon(Daemon&&) = delete;
    Daemon&& operator=(Daemon&&) = delete;
};
#endif
