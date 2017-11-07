#ifndef __Daemon_H__
#define __Daemon_H__

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

namespace Application
{
namespace Daemon
{
void Create(const std::function<void(void)>& init)
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
}
}
#endif
