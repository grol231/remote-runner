#include "config.h"

using namespace std;
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
        string command(argv[i]);
        if(command.compare("-nodaemon") == 0)
        {               
            is_daemon_ = false;
        }
        if(string::npos != command.find("-path="))
        {                           
            string path(command,
                command.find('=')+1, command.size()-command.find('=')-1);                
            cout << path << endl;
            ReadConfigFile(path);
        }
        if(command.compare("-log") == 0)
        {
            logging_ = true;
        }
        if(string::npos != command.find("-timeout="))
        {
            string str_timeout(command,
                command.find('=')+1, command.size()-command.find('=')-1);                
            timeout_ = static_cast<boost::posix_time::seconds>(
                    	stoi(str_timeout));
        }
        if(string::npos != command.find("-port="))
        {
            string str_port(command, command.find('=')+1,
                 command.size()-command.find('=')-1);
            port_ = static_cast<unsigned short>(stoi(str_port));
        }
    }
}
unsigned short Config::Port() const
{
    return port_;
}
boost::posix_time::seconds Config::Timeout() const
{
    return timeout_;
}
bool Config::Logging() const
{
    return logging_;
}
const vector<string>& Config::AllowCommands() const
{
    return allow_commands_;    
}
bool Config::isDaemon() const
{
    return is_daemon_;
}
void Config::ReadConfigFile(const string& path)
{
    string command;
    ifstream myfile (path.c_str());
    if (myfile.is_open())
    {
        while (getline(myfile,command))
        {
            if(command.size() <= 1)
                continue;
            cout << command << endl;
            allow_commands_.push_back(command);
        }
        myfile.close();
    }
}

