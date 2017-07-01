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
/*
using shared_mc = attrs::mutable_constant<
    std::string,
    boost::shared_mutex,
    boost::unique_lock<boost::shared_mutex>,
    boost::shared_lock<boost::shared_mutex>>;
    */
//TODO: Global variable! Fix it!
//shared_mc logType("LogRecord");
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
    RunningCommandCounter(0),
    NotRunningCommandCounter(0),
    CompletedCommandCounter(0),
    CompletedCompulsorilyCommandCounter(0),
    DownloadedData(0),
    UploadedData(0)
    {}
    unsigned long long int ConnectID;
    unsigned long long int RunningCommandCounter;
    unsigned long long int NotRunningCommandCounter;
    unsigned long long int CompletedCommandCounter;
    unsigned long long int CompletedCompulsorilyCommandCounter;
    unsigned long long int DownloadedData;
    unsigned long long int UploadedData;
    void AddCounters(const Statistic& s)
    {
        RunningCommandCounter += s.RunningCommandCounter;
        NotRunningCommandCounter += s.NotRunningCommandCounter;
        CompletedCommandCounter += s.CompletedCommandCounter;
        CompletedCompulsorilyCommandCounter += s.CompletedCompulsorilyCommandCounter;        
    }

};
//TODO: I must make two log files. Log will use mutable_constant for
// file name. Mutable_const has shared_mutex.
//BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(my_logger, src::logger_mt) WTF?
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
    //src::logger l;
    logging::core::get()->set_filter
    (   
        logging::trivial::severity >= logging::trivial::info
    );
};
//TODO: use stream!
static std::string ToString(const LogRecord& record)//Why static?
{
    return std::string("ConnectID #" + 
            std::to_string(record.ConnectID) + "\n" +
            "Command : " + record.Command + "\n" + 
            "Condition : " + record.Condition + "\n" +
           "Result : " +  record.Result +"\n"+ 
           "Note : " + record.Note);
};
//TODO: use stream! refactoring!
static std::string ToString(const Statistic& s)
{
    std::string str;
    str += "ConnectID #" + std::to_string(s.ConnectID) + "\n";
    str += "Command Statistic:\n    stoped = "; 
    str += std::to_string(s.RunningCommandCounter);
    str += "\n    not running = ";
    str += std::to_string(s.NotRunningCommandCounter);
    str += "\n    completed = ";
    str += std::to_string(s.CompletedCommandCounter);
    str += "\n    completed compulsorily = ";
    str += std::to_string(s.CompletedCompulsorilyCommandCounter);
    str += "\nTraffic Statistic:\n    downlowded data = ";
    str += std::to_string(s.DownloadedData);
    str += "\n    uploaded data = ";
    str += std::to_string(s.UploadedData);
    str += "\n";
    return str;
};
}
#endif
