#include <iostream>
#include <memory>
#include <exception>
#include <new>
#include "config.h"
#include "application.h"
#include "log.h"

//using namespace boost::log;

int main(int argc, char* argv[])
{
    //TODO: Rewrite all error handling. I must make only handling error code.
    //Error handling will be in log.
    try
    {/*
        std::unique_ptr<Config> config(new Config(argc, argv));
        logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );
*/
        //TODO:Use shared_ptr!
        //Application app(config);

        Log log;
        BOOST_LOG_TRIVIAL(trace);
        BOOST_LOG_TRIVIAL(info);       
        BOOST_LOG_TRIVIAL(fatal);

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
