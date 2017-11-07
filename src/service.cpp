#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <memory>
#include <iostream>
#include "service.h"

using namespace std;
using namespace boost::asio;

namespace Application
{
Service::Service(shared_ptr<ip::tcp::socket> sock,     
                 io_service& ios,
                 unsigned long long int connect_id,
                 src::severity_logger<logging::trivial::severity_level>& log,
                 bool logging) :
        sock_(sock),
        buffer_(make_shared<boost::asio::streambuf>()),
        ios_(ios),
        timer_(ios_),
        statistic_(),
        log_(log),
        logging_(logging)
{
    statistic_.ConnectID = connect_id;
}
Service::~Service()
{
    if(logging_)
    {
        ofstream fout("statistic.txt", ios_base::app);
        fout << Logging::ToString(statistic_);
        fout.close();
    }        
}
void Service::StartHandling(shared_ptr<Config> config)
{
   weak_ptr<Service> weakSelf = shared_from_this();
   handling_ = [weakSelf, config](){
       shared_ptr<Service> self = weakSelf.lock();
       async_read_until(*self->sock_.get(),
           *self->buffer_.get(),
           '\n',
           [self, config](const boost::system::error_code& ec,
                          size_t bytes_transferred)
          { 
                 if(0 == ec)
                 {
                     if(1 < bytes_transferred)
                     {                              
                         self->OnRequestReceived(bytes_transferred, config);
                     }
                     self->buffer_->consume(bytes_transferred);
                     self->handling_();
                 }
                 else
                 {
                    cout << "Async operation error!" << endl;
                 }
          }
       );
   };
   handling_();
}
void Service::OnRequestReceived(size_t bytes_transferred,
                                shared_ptr<Config> config)
{
    statistic_.DownloadedBytes += bytes_transferred;
    response_ = ProcessRequest(buffer_, config);
    async_write(*sock_.get(),
        	buffer(response_),
        [this](const boost::system::error_code& ec,
            	size_t bytes_transferred)
        {
            OnResponseSent(ec, bytes_transferred);
        }
    );
}
void Service::OnResponseSent(const boost::system::error_code& ec,
                             size_t bytes_transferred)
{
    if (ec != 0)
    {
        cout << "Error occured! Error code = "
            << ec.value()
            << ". Message: " << ec.message();
    }
    statistic_.UploadedBytes += bytes_transferred;
}

string Service::ProcessRequest(shared_ptr<boost::asio::streambuf> buffer,
                                    shared_ptr<Config> config)
{
    istream is(buffer.get());
    vector<string> args;
    while(is)
    {
        string cmd;
        is >> cmd;         
        if(cmd.empty())
            break;            
        args.push_back(cmd);
    }
    Logging::LogRecord record;
    record.Command = args[0];
    record.Condition = "start";        
    auto list = config->AllowCommands();
    if(list.empty())
        cout << "List is empty!" << std::endl;
    if(!list.empty() &&
        list.end() 
            == 
        find(list.begin(), list.end(), args[0]))
    {
        string message = "not allow command!";
        record.Result = "fail";
        record.Note = message; 
        ++statistic_.FailedLaunches;
        BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
        return message + "\n";
    }
    pid_t pid = fork();
    int err(0);
    if(pid < 0)
    {
        string message = ProcessError(errno);
        record.Result = "fail";
        record.Note = message; 
        ++statistic_.FailedLaunches;
        BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
        return record.Result + "\n";
    }
    if(!pid)
    {
        char** argv = CreateArgv(args);
        execvp(args[0].c_str(),argv );
        perror("child");
        exit(1);
    }
    else
    {            
        ++statistic_.Launches;
        record.Result = "success";
        size_t num = timer_.expires_from_now(config->Timeout());
        if(0!=num)
            cout << "Too late! Timer already expires!" <<  endl;
        timer_.async_wait([this,pid](const boost::system::error_code& ec){
            unsigned int result = kill(pid,SIGKILL);
            if(result == 0)                
                ++statistic_.ForcedTerminations;                
            else                
                ++statistic_.Terminations;                
        });            
    }
    if(logging_)    
        BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
    return record.Result+"\n";
}
char** Service::CreateArgv(const vector<string>& args) const {
    char** argv = new char*[args.size() + 1];
    for (size_t i = 0; i < args.size(); ++i) {
        argv[i] = new char[args[i].length() + 1];
        strcpy(argv[i], args[i].c_str());
    }        
    argv[args.size()] = nullptr;
    return argv;
}
string Service::ProcessError(int err)
{
    string result;
    switch(err)
    {
        case EAGAIN:
            result = "The system lacked the necessary \
                resources to create another process.";
        break;
        case ENOMEM:
            result = "Insufficient storage is available.";
        break;
        default:
            result = "Uknown error code.";
    }
    return result;
}
}
