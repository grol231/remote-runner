#ifndef __Log_H__
#define __Log_H__

#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
//namespace logging = boost::log;

#define LOG(e) RR::Log(e)

namespace RR
{
    const std::string LOGFILE = "log.txt";
    //optimization
    void Log(std::string entry)
    {
        //FIXIT: It is very bad code.
        static bool flag(true);
        if(flag)
        {
            boost::log::add_file_log("log.txt");
            flag = false;
            std::cout << "Logfile initialization!" << std::endl;
        }
        else
        {
            std::cout << "Logfile is already initiailzed!" << std::endl;
        }
        BOOST_LOG_TRIVIAL(info) << entry;
    }
}
#endif
