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
//       Logging::InitializeLog();        
        std::unique_ptr<Config> config(new Config(argc, argv));
      //TODO:Use shared_ptr!
        Application app(config);
        app.Initialize();
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
