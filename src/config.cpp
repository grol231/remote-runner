#include "config.h"

const boost::posix_time::seconds DEFAULT_TIMEOUT(5);

Config::Config(int argc, char* argv[])
    :port_(DEFAULT_PORT_NUM),
    allow_commands_(),
    timeout_(5),
    logging_(false),
    is_daemon_(true)
{
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
            std::cout << path << std::endl;
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
Config::~Config(){}
const unsigned Config::Port() const
{
    return port_;
}
const boost::posix_time::seconds Config::Timeout() const
{
    return timeout_;
}
const bool Config::Logging() const
{
    return logging_;
}
const std::vector<std::string>& Config::AllowCommands() const
{
    return allow_commands_;    
}
const bool Config::isDaemon() const
{
    return is_daemon_;
}
void Config::ReadConfigFile(std::string& path)
{
    std::string command;
    std::ifstream myfile (path.c_str());
    if (myfile.is_open())
    {
        while (getline(myfile,command))
        {
            if(command.size() <= 1)
                continue;
            std::cout << command << std::endl;
            allow_commands_.push_back(command);
        }
        myfile.close();
    }
}

