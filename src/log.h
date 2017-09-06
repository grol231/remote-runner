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

namespace Logging
{
static const char* DEFAULT_LOG_FILE = "log.txt";
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
    Statistic();
    unsigned long long int ConnectID;
    unsigned long long int Launches;
    unsigned long long int FailedLaunches;
    unsigned long long int Terminations;
    unsigned long long int ForcedTerminations;
    unsigned long long int DownloadedBytes;
    unsigned long long int UploadedBytes;
};
static void InitializeLog();
static std::string ToString(const LogRecord& record);
static std::string ToString(const Statistic& s);
};
#endif
