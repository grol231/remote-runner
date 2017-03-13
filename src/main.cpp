#include <iostream>
#include <memory>
#include <exception>
#include <new>
#include "config.h"
#include "application.h"
#include "log.h"

int main(int argc, char* argv[])
{
    //TODO: Rewrite all error handling. 
    //I must make only handling error code.
    //Error handling will be in log.
    try
    {
        pid_t pid = fork();
        if(pid < 0)
        {
            std::cout << "can't fork" << std::endl;            
            exit(1);
        }
        else
        {
            if(pid != 0)            
            {
                std::cout << "I am in the parent." << std::endl;
                exit(0);
            }
            else
            {
                std::cout << "I am in the child." << std::endl;
            }                
        }
        setsid();
        unsigned int numFiles = sysconf(_SC_OPEN_MAX);
        for(unsigned int i = 0; i < numFiles; ++i)
        {
            close(i);
        }
        std::unique_ptr<Config> config(new Config(argc, argv));
      //TODO:Use shared_ptr!
        Application app(config);
    }
    catch(boost::system::system_error& e)
    {
        std::cout << "Error occured! Error code = "
			<< e.code() << ". Message: "
			<< e.what() << std::endl;
    }
    catch(std::exception& e)
    {
        std::cout << "Error occured!"
        ". Message: "<< e.what() << std::endl;
    }
    catch(...)
    {

        std::cout << "Unkown error occured!" << std::endl;
    }
	return 0;
}
