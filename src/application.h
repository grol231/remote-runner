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
    Application(std::unique_ptr<Config>& config)
        :config_(config)
    {
        std::cout << "Create application" << std::endl;
    }
    void Initialize()
    {
        std::cout << "Application::Initialize()" << std::endl;
        if(config_->isDaemon())
            Daemon([this](){DoInitialize();});        
        else
            DoInitialize();
    }
    void DoInitialize()
    {
        std::cout << "Application::DoInitialize()" << std::endl;
        std::unique_ptr<Server> srv(new Server());
		std::function<void(void)> stopServer = [&srv]()
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
        srv->Start(config_->Port(), thread_pool_size,
            config_->AllowCommands(), config_->Timeout());
        th->join();
    }
private:
    std::unique_ptr<Config>& config_;
};
#endif
