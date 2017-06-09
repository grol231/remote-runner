#ifndef __Runner_H__
#define __Runner_H__

class Runner 
{
public:
    Runner(){}
    void Power()
    {/*
            char** argv = CreateArgv(args);
            execvp(command.c_str(),argv );
            std::string message = ProcessError(errno);
            response += message;
            record.Result = "fail";
            record.Note = message;             
            BOOST_LOG_SEV(log_,logging::trivial::info) << Logging::ToString(record);
            perror("child");
            exit(1);
     */
    }
    void CreateArgv(){}
    void ProcessError(){}
    void Kill()
    {
        /*
         size_t num = timer_.expires_from_now(config->Timeout());
            if(0!=num)
                std::cout << "Too late! Timer already expires!" <<  std::endl;
            timer_.async_wait([this,pid](const boost::system::error_code& ec){
                unsigned int result = kill(pid,SIGKILL);
                if(result == 0)                
                    ++statistic_.CompletedCompulsorilyCommandCounter;                
                else                
                    ++statistic_.CompletedCommandCounter;                
                std::cout << "Kill child process! pId:" << pid <<  std::endl;
            });            
         */   

    }
    
    char** CreateArgv(const std::vector<std::string>& args) const 
    {
        char** argv = new char*[args.size() + 1];
        for (size_t i = 0; i < args.size(); ++i) {
            argv[i] = new char[args[i].length() + 1];
            strcpy(argv[i], args[i].c_str());
        }        
        argv[args.size()] = nullptr;
        return argv;
    }
    /* 
    std::string processerror(int err)
    {
        std::string result;
        switch(err)
        {
            case eagain:
                result = "the system lacked the necessary \
                    resources to create another process.";
            break;
            case enomem:
                result = "insufficient storage is available.";
            break;
            default:
                result = "uknown error code.";
        }
        return result;
    }


    
    
     * */
    Runner& operator=(const Runner&) = delete;
    Runner(const Runner&) = delete;
};

#endif
