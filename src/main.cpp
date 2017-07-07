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
        std::shared_ptr<Config> config(new Config(argc, argv));
        std::unique_ptr<Application> app(std::make_unique<Application>());
        app->Initialize(config);
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
