#include <iostream>
#include <thread>
#include "server.h"

const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;

int main()
{
	unsigned short port_num = 2001;

	try
	{
		Server srv;
		std::function<void(void)> f = [&srv]()
		{
            std::string msg;
            while(true)
            {
                std::cin >> msg;
                if(std::string::npos != msg.find('q'))
                {
                    srv.Stop();
                    break;
                }
            }
		};
		std::thread th(f);
		unsigned int thread_pool_size =
			std::thread::hardware_concurrency() * 2;
		if (thread_pool_size == 0){
			thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
        }
        else
        {
            std::cout << "Thread pool size:" << thread_pool_size << std::endl;
        }
        srv.Start(port_num, thread_pool_size);
        th.join();
	}
	catch (boost::system::system_error &e)
	{
		std::cout << "Error occured! Error code = "
			<< e.code() << ". Message: "
			<< e.what();
	}
	return 0;
}
