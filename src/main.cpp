#include <iostream>
#include <memory>
#include <exception>
#include <new>
#include "config.h"
#include "application.h"

//boost::asio::io_service io_s;
//boost::asio::deadline_timer t(io_s, boost::posix_time::seconds(5));
//
//void call(){
//    t.async_wait([](const boost::system::error_code& ec){
//            if(ec == boost::asio::error::operation_aborted)
//                std::cout << "Operation aborted!" << std::endl;
//            else
//                std::cout << "Expired timer!" << std::endl;
//            size_t num = t.expires_from_now(boost::posix_time::seconds(5));
//            if(0 != num)
//                std::cout << "Too late! Timer has bean alwayes expire!" << "num:" << num << std::endl;
//            else
//                std::cout << "Timer works!" << "num:" << num << std::endl;
//            call();
//        });
//}

int main(int argc, char* argv[])
{
    //TODO: Rewrite all error handling. I must make only handling error code.
    //Error handling will be in log.
    try
    {
        std::unique_ptr<Config> config(new Config(argc, argv));
        //TODO:Use shared_ptr!
        Application app(config);
        //size_t num = t.expires_from_now(boost::posix_time::seconds(5));
//        t.async_wait([](const boost::system::error_code& ec){
//            if(ec == boost::asio::error::operation_aborted)
//                std::cout << "Operation aborted!" << std::endl;
//            else
//                std::cout << "Expired timer!" << std::endl;
//            call();
//
//        });
//        //size_t num = t.expires_from_now(boost::posix_time::seconds(1000));
////        if(0 != num)
////            std::cout << "Too late! Timer has bean alwayes expire!" << "num:" << num << std::endl;
////        else
////            std::cout << "Timer works!" << "num:" << num << std::endl;
//        io_s.run();
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
