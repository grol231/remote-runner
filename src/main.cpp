#include <iostream>
#include <memory>
#include <exception>
#include <new>
#include "config.h"
#include "application.h"

int main(int argc, char* argv[])
{
    try
    {
        std::unique_ptr<Config> config(new Config(argc, argv));
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
