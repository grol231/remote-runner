#include "log.h"


Logging::Statistic::Statistic():
    ConnectID(0),
    Launches(0),
    FailedLaunches(0),
    Terminations(0),
    ForcedTerminations(0),
    DownloadedBytes(0),
    UploadedBytes(0)
    {} 
void Logging::InitializeLog(bool logging)
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
std::string Logging::ToString(const LogRecord& record)
{
    return std::string("ConnectID #" + 
            std::to_string(record.ConnectID) + "\n" +
            "Command : " + record.Command + "\n" + 
            "Condition : " + record.Condition + "\n" +
           "Result : " +  record.Result +"\n"+ 
           "Note : " + record.Note);
};
std::string Logging::ToString(const Statistic& s)
{
    std::cout << "Logging::ToString(Statistic)" << std::endl;
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


