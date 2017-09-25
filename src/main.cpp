#include <iostream>
#include <memory>
#include <exception>
#include "config.h"
#include "application.h"

int main(int argc, char* argv[])
{
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
