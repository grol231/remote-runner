#ifndef __Config_H__
#define __Config_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

const unsigned short DEFAULT_PORT_NUM = 12345;//TODO This const puts to class.
const boost::posix_time::seconds DEFAULT_TIMEOUT(5);

class Config
{
public:
    Config(int argc, char* argv[])
    :port_(DEFAULT_PORT_NUM),
    allow_commands_(),
    timeout_(DEFAULT_TIMEOUT),
    logging_(false)
    {
        for(unsigned i = 0; argc > i; ++i)
        {
            std::string command(argv[i]);
            if(std::string::npos != command.find("-nodaemon"))
            {
                std::cout << "Found -nodaemon" << std::endl;
                is_daemon_ = false;
            }
            else
            {
                is_daemon_ = true;
            }
            if(std::string::npos != command.find("-path="))
            {
                //std::cout << "Found -path=!" << std::endl;
                std::string path(command,
                    command.find('=')+1, command.size()-command.find('=')-1);
                //std::cout << "path: " << path << std::endl;
                ReadConfigFile(path);
            }
            if(std::string::npos != command.find("-log"))
            {
                logging_ = true;
            }
            if(std::string::npos != command.find("-timeout="))
            {
                std::string str_timeout(command,
                    command.find('=')+1, command.size()-command.find('=')-1);
                //TODO: Rewrite type conversion.
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
        std::cout << "timeout:" << timeout_ << std::endl;
        std::cout << "logging:"//TODO:Rewrite!
            << std::string((logging_)?"true":"false") << std::endl;
        std::cout << "port:" << port_ << std::endl;
        std::cout << "allow commands:";
        for(auto& i : allow_commands_)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config() = delete;
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
                if(command.size() <= 1)//FIXME: This is crutch!
                    continue;
                allow_commands_.push_back(command);
                //std::cout << command << std::endl;
            }
            myfile.close();
        }
    }
    unsigned short port_;
    std::vector<std::string> allow_commands_;//TODO:Use shared_ptr!
    boost::posix_time::seconds timeout_; //FIXME: std::chrono
    bool logging_;
    bool is_daemon_;
};
#endif
