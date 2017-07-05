#ifndef __Registrar_H__
#define __Registrar_H_

#include <memory>
#include <string>

class Registrar
{
public:
    Registrar():
        records_(),
        statistic_()
    {}
    Registrar(const Registrar&) = delete;
    Registrar& operator=(const Registrar&) = delete;

    void RegisterRun(){}
    void RegisterFailRun(){}
    void RegisterForcedTermination(){}
    void RegisterTermination(){}
    void RegisterDownload(){}
    void RegisterUpload(){}
    std::unique_ptr<std::string> Print(){}
private:
    struct CommandInfo
    {   
        std::string Name;
        std::vector<std::string> Arguments;
        bool WasLaunched;
        bool WasTerminated;
        bool WasKilled; 
        std::string Note;    
    };
    struct Statistic
    {   
        unsigned long long int RunningCommandCounter;
        unsigned long long int NotRunningCommandCounter;
        unsigned long long int CompletedCommandCounter;
        unsigned long long int CompletedCompulsorilyCommandCounter;
        unsigned long long int DownloadedData;
        unsigned long long int UploadedData;
    };
    std::vector<CommandInfo> records_;
    Statistic statistic_;
    unsigned long long int connnect_id_;
};
#endif
