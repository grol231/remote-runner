#include "log.h"
#include <fstream>

using namespace std;

namespace Application
{
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
    std::ofstream fout("statistic.txt"); //clean file
    fout.close();
};
string Logging::ToString(const LogRecord& record)
{
    return string("ConnectID #" + 
            to_string(record.ConnectID) + "\n" +
            "Command : " + record.Command + "\n" + 
            "Condition : " + record.Condition + "\n" +
           "Result : " +  record.Result +"\n"+ 
           "Note : " + record.Note);
};
string Logging::ToString(const Statistic& s)
{
    string str;
    str += "\nConnectID #" + to_string(s.ConnectID) + "\n";
    str += "Command Statistic:\n    Terminations = "; 
    str += to_string(s.Terminations);
    str += "\n    Forced Terminations = ";
    str += to_string(s.ForcedTerminations);
    str += "\n    Launches = ";
    str += to_string(s.Launches);
    str += "\n    Failed Launches = ";
    str += to_string(s.FailedLaunches);
    str += "\nTraffic Statistic:\n    Downlowded Bytes = ";
    str += to_string(s.DownloadedBytes);
    str += "\n    Uploaded Bytes = ";
    str += to_string(s.UploadedBytes);
    str += "\n";
    return str;
};
}

