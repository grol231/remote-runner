#ifndef __Config_H__
#define __Config_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

const unsigned short DEFAULT_PORT_NUM = 12345;
const boost::posix_time::seconds DEFAULT_TIMEOUT(5);

class Config
{
public:
    Config(int argc, char* argv[])
    :port_(DEFAULT_PORT_NUM),
    allow_commands_(),
    timeout_(DEFAULT_TIMEOUT),
    logging_(false),
    is_daemon_(true)
    {
        std::cout << "Found ";
        for(unsigned i = 0; argc > i; ++i)
        {
            std::string command(argv[i]);
            if(command.compare("-nodaemon") == 0)
            {               
                is_daemon_ = false;
            }
            if(std::string::npos != command.find("-path="))
            {                
                std::string path(command,
                    command.find('=')+1, command.size()-command.find('=')-1);                
                ReadConfigFile(path);
            }
            if(command.compare("-log") == 0)
            {
                logging_ = true;
            }
            if(std::string::npos != command.find("-timeout="))
            {
                std::string str_timeout(command,
                    command.find('=')+1, command.size()-command.find('=')-1);                
                timeout_ = static_cast<boost::posix_time::seconds>(
                        std::stoi(str_timeout));
            }
            if(std::string::npos != command.find("-port="))
            {
                std::string str_port(command, command.find('=')+1,
                     command.size()-command.find('=')-1);
                port_ = static_cast<unsigned>(std::stoi(str_port));
            }
        }
    }
    ~Config()
    {
    }
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config() = delete;
    Config(Config&&) = delete;
    Config&& operator=(Config&&) = delete;
    const unsigned Port() const
    {
        return port_;
    }
    const boost::posix_time::seconds Timeout() const
    {
        return timeout_;
    }
    const bool Logging() const
    {
        return logging_;
    }
    const std::vector<std::string>& AllowCommands() const
    {
        return allow_commands_;    
    }
    const bool isDaemon() const
    {
        return is_daemon_;
    }
private:
    void ReadConfigFile(std::string& path)
    {
        std::string command;
        std::ifstream myfile (path.c_str());
        if (myfile.is_open())
        {
            while (getline(myfile,command))
            {
                if(command.size() <= 1)
                    continue;
                allow_commands_.push_back(command);
            }
            myfile.close();
        }
    }
    unsigned short port_;
    std::vector<std::string> allow_commands_;
    boost::posix_time::seconds timeout_;
    bool logging_;
    bool is_daemon_;
};
#endif
