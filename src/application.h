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
        std::cout << "Application::DoInitialize" << std::endl;
        std::shared_ptr<Server> srv(new Server());
	    srv->Start(2);
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
        th->join();
        std::cout << "End of Application::DoInitialize" << std::endl;
    }
};
#endif
