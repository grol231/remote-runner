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
namespace Logging
{
struct LogRecord
{
    std::string Command;
    std::string Condition;
    std::string Result;
    std::string Note;    
};

struct Statistic
{     
    unsigned long long int StopedCommandCounter;
    unsigned long long int NotRunningCommandCounter;
    unsigned long long int CompletedCommandCounter;
    unsigned long long int CompletedCompulsorilyCommandCounter;
    unsigned long long int DownloadedData;
    unsigned long long int UploadedData;
};

class Log 
{
public:
    Log()
    {
//        logging::add_file_log(DEFAULT_LOG_FILE);

        logging::add_file_log(
            keywords::file_name = DEFAULT_LOG_FILE,
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation = 
                sinks::file::rotation_at_time_point(0,0,0),
            keywords::format = 
                "RecordID #%LineID%\n%TimeStamp%\n%Message%"        
        );

        logging::core::get()->set_filter
        (
            logging::trivial::severity >= logging::trivial::info
        );
    }
};
//TODO: use stream!
static std::string ToString(const LogRecord& record)//Why static?
{
    return std::string("Command : " + record.Command + "\n" + 
            "Condition : " + record.Condition + "\n" +
           "Result : " +  record.Result +"\n"+ 
           "Note : " + record.Note);
};
//TODO: use stream! refactoring!
static std::string ToString(const Statistic& s)
{
    std::string str;
    str += "Command Statistic:\n    stoped = "; 
    str += std::to_string(s.StopedCommandCounter);
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
