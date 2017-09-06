#include "application.h"

const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;

void Application::Initialize(std::shared_ptr<Config> config)
{
    if(config->isDaemon())
        Daemon([this,config](){DoInitialize(config);});        
    else
        DoInitialize(config);
}
void Application::DoInitialize(std::shared_ptr<Config> config)
{
    std::shared_ptr<Server> srv(new Server());
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
    srv->Start(config, thread_pool_size);
    th->join();
}

