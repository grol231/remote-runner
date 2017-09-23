#ifndef __Application_H__
#define __Application_H__

#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <thread>
#include "server.h"
#include "config.h"
#include "daemon.h"


class Application
{
public:
    Application()
    {}
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;    
    Application(Application&&) = delete;
    Application&& operator=(Application&&) = delete;
    ~Application(){}
    void Initialize(std::shared_ptr<Config> config);
    void DoInitialize(std::shared_ptr<Config> config);
    const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;
};
#endif
