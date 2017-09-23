#ifndef __Config_H__
#define __Config_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

class Config
{
public:
    const unsigned short DEFAULT_PORT_NUM = 12345;
    static const boost::posix_time::seconds DEFAULT_TIMEOUT;
    Config(int argc, char* argv[]);
    ~Config();
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config() = delete;
    Config(Config&&) = delete;
    Config&& operator=(Config&&) = delete;
    const unsigned Port() const;
    const boost::posix_time::seconds Timeout() const;
    const bool Logging() const;
    const std::vector<std::string>& AllowCommands() const;
    const bool isDaemon() const;
private:
    void ReadConfigFile(std::string& path);
    unsigned short port_;
    std::vector<std::string> allow_commands_;
    boost::posix_time::seconds timeout_;
    bool logging_;
    bool is_daemon_;
};
#endif
