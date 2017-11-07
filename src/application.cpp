#include <functional>
#include <string>
#include <thread>
#include "server.h"
#include "daemon.h"
#include "application.h"

using namespace std;

namespace Application
{
void Initialize(shared_ptr<Config> config)
{
    if(config->isDaemon())
        Daemon::Create([config](){DoInitialize(config);});        
    else
        DoInitialize(config);
}
void DoInitialize(shared_ptr<Config> config)
{
    shared_ptr<Server> srv(new Server(config->Logging()));
    function<void(void)> stopServer = [srv]()
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
    unique_ptr<thread> th(new thread(stopServer));
    unsigned int thread_pool_size =
        thread::hardware_concurrency() * 2;
    if (thread_pool_size == 0)
    {
        thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
    }
    srv->Start(config, thread_pool_size);
    th->join();
}

}

