#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

const char* DEFAULT_LOG_FILE = "log.txt";

class Log 
{
public:
    Log()
    {
        logging::add_file_log(DEFAULT_LOG_FILE);
/*
        logging::add_file_log(
            keywords::file_name = DEFAULT_LOG_FILE,
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation = 
                sinks::file::rotation_at_time_point(0,0,0),
            keywords::format = "[%TimeStamp%]: %Message%"
        
        );
*/
        logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );
    }
};

