#ifndef __Application_H__
#define __Application_H__

#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <thread>
#include "server.h"


class Application
{
public:
    Application()
    {}
    void Initialize()
    {
            DoInitialize();
    }
    void DoInitialize()
    {
        std::shared_ptr<Server> srv(new Server());
	    srv->Start(2);
    }
};
#endif
