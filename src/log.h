#ifndef __Log_H__
#define __Log_H__

#include <iostream>
#include <boost/thread/shared_mutex.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/basic_logger.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

const char* DEFAULT_LOG_FILE = "log.txt";
namespace Logging
{
struct LogRecord
{
    unsigned long long int ConnectID;
    std::string Command;
    std::string Condition;
    std::string Result;
    std::string Note;    
};
struct Statistic
{   
  Statistic():
    ConnectID(0),
    Launches(0),
    FailedLaunches(0),
    Terminations(0),
    ForcedTerminations(0),
    DownloadedBytes(0),
    UploadedBytes(0)
    {}
    unsigned long long int ConnectID;
    unsigned long long int Launches;
    unsigned long long int FailedLaunches;
    unsigned long long int Terminations;
    unsigned long long int ForcedTerminations;
    unsigned long long int DownloadedBytes;
    unsigned long long int UploadedBytes;
};
static void InitializeLog()
{
    logging::add_file_log(
        keywords::file_name = DEFAULT_LOG_FILE,
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = 
            sinks::file::rotation_at_time_point(0,0,0),
        keywords::format = 
            "Record №%LineID%\n%TimeStamp%\n%Message%\n" 
    );
    logging::add_common_attributes();
    logging::core::get()->set_filter
    (   
        logging::trivial::severity >= logging::trivial::info
    );
};
std::string ToString(const LogRecord& record)
{
    return std::string("ConnectID #" + 
            std::to_string(record.ConnectID) + "\n" +
            "Command : " + record.Command + "\n" + 
            "Condition : " + record.Condition + "\n" +
           "Result : " +  record.Result +"\n"+ 
           "Note : " + record.Note);
};
std::string ToString(const Statistic& s)
{
    std::string str;
    str += "ConnectID #" + std::to_string(s.ConnectID) + "\n";
    str += "Command Statistic:\n    Terminations = "; 
    str += std::to_string(s.Terminations);
    str += "\n    Forced Terminations = ";
    str += std::to_string(s.ForcedTerminations);
    str += "\n    Launches = ";
    str += std::to_string(s.Launches);
    str += "\n    Failed Launches = ";
    str += std::to_string(s.FailedLaunches);
    str += "\nTraffic Statistic:\n    Downlowded Bytes = ";
    str += std::to_string(s.DownloadedBytes);
    str += "\n    Uploaded Bytes = ";
    str += std::to_string(s.UploadedBytes);
    str += "\n";
    return str;
};
}
#endif
