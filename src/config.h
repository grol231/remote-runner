#ifndef __Config_H__
#define __Config_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const unsigned short PORT_NUM = 12345;

class Config
{
public:
    Config(int argc, char* argv[])
    :port_(PORT_NUM),
    allow_commands_(),
    timeout_(0),
    logging_(false)
    {
        for(unsigned i = 0; argc > i; ++i)
        {
            std::string command(argv[i]);
            if(std::string::npos != command.find("-path="))
            {
                std::string path(command, command.find('='), command.size()-command.find('='));
                ReadConfigFile(path);
            }
            if(std::string::npos != command.find("-log"))
            {
                logging_ = true;
            }
            if(std::string::npos != command.find("-timeout="))
            {
                std::string str_timeout(command, command.find('='), command.size()-command.find('='));
                timeout_ = static_cast<unsigned>(std::stoi(str_timeout));
            }
            if(std::string::npos != command.find("-port="))
            {
                std::string str_port(command, command.find('='), command.size()-command.find('='));
                port_ = static_cast<unsigned>(std::stoi(str_port));
            }
        }
    }
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config() = delete;
    const unsigned Port() const
    {
        return port_;
    }
    const unsigned Timeout() const
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
private:
    void ReadConfigFile(std::string& path)
    {
        std::string command;
        std::ifstream myfile (path.c_str());
        if (myfile.is_open())
        {
            while (getline(myfile,command))
            {
              allow_commands_.push_back(command);
              std::cout << command << std::endl;
            }
            myfile.close();
        }
    }
    unsigned short port_;
    std::vector<std::string> allow_commands_;
    int timeout_; //FIXME: std::chrono
    bool logging_;
};
#endif
