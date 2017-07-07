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

const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;

class Application
{
public:
    Application()
    {}
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;    
    Application(Application&&) = delete;
    Application&& operator=(Application&&) = delete; 
    ~Application()
    {
        std::cout << "Application destroyed!" << std::endl;
    }
    //TODO: delete copy constructor and assign operator!
    void Initialize(std::shared_ptr<Config> config)
    {
        if(config->isDaemon())
            Daemon([this,config](){DoInitialize(config);});        
        else
            DoInitialize(config);
    }
    void DoInitialize(std::shared_ptr<Config> config)
    {
        std::shared_ptr<Server> srv(new Server());
        //TODO: make to stop server in one click
		std::function<void(void)> stopServer = [srv]()
		{
            std::string msg;
            while(true)
            {
                std::cin >> msg;
                if(std::string::npos != msg.find('q'))
                {
                    srv->Stop();
                    break;
                }
            }
		};
		std::unique_ptr<std::thread> th(new std::thread(stopServer));
		unsigned int thread_pool_size =
			std::thread::hardware_concurrency() * 2;
		if (thread_pool_size == 0)
        {
			thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
        }
        else
        {
            std::cout << "Thread pool size:" 
                << thread_pool_size << std::endl;
        }
        srv->Start(config, thread_pool_size);
        th->join();
    }
};
#endif
