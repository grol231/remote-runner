#include <iostream>
#include <memory>
#include <exception>
#include <new>
#include "config.h"
#include "application.h"
#include "log.h"

int main(int argc, char* argv[])
{
    //TODO: Rewrite all error handling. I must make only handling error code.
    //Error handling will be in log.
    try
    {
        std::unique_ptr<Config> config(new Config(argc, argv));
      //TODO:Use shared_ptr!
      //Application app(config);
        logging::add_common_attributes();
        using namespace logging::trivial;
        src::severity_logger<severity_level> lg;
        Logging::Log log;
        Logging::Statistic s;
        Logging::LogRecord r;
        s.StopedCommandCounter = 5;
        s.NotRunningCommandCounter = 6;
        s.CompletedCommandCounter = 7;
        s.CompletedCompulsorilyCommandCounter = 8;
        s.DownloadedData = 99;
        s.UploadedData = 999;
        r.Command = "gnome-terminal";
        r.Condition = "start";
        r.Result = "success";
        r.Note = "This is good program.";
        BOOST_LOG_SEV(lg, trace) << Logging::ToString(s);
        BOOST_LOG_SEV(lg, info) << Logging::ToString(s);  
        BOOST_LOG_SEV(lg, fatal) << Logging::ToString(r);
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
