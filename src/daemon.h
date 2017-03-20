#ifndef __Daemon_H__
#define __Daemon_H__
#include <iostream>

class Daemon
{
    Daemon(std::function<void(void)>& init)
    {
        init();
    }
};
#endif
