#ifndef __Registrar_H__
#define __Registrar_H__

#include <memory>
#include <string>
#include <vector>

class Registrar
{
public:
    Registrar(unsigned long long int connect_id):
        records_(),
        statistic_(),
        connect_id_(connect_id)
    {
        statistic_.RunCounter = 0;
        statistic_.FailRunCounter = 0;
        statistic_.TerminationCounter = 0;
        statistic_.ForcedTerminationCounter = 0;
        statistic_.DownloadedDataCounter = 0;
        statistic_.UploadedDataCounter = 0;
    }
    Registrar(const Registrar&) = delete;
    Registrar& operator=(const Registrar&) = delete;
    Registrar(Registrar&&) = delete;
    Registrar&& operator=(Registrar&&) = delete;

    void RegisterRun(std::string& name, std::vector<std::string>& args)
    {
        ++statistic_.RunCounter;
        CommandInfo info;
        info.Name = name;
        info.Arguments = args;
        info.WasLaunched = true;
        info.WasTerminated = false;
        info.WasKilled = false;
        //TODO: optimization! use std::move()!
        records_.push_back(info);
    }
    void RegisterFailRun(std::string& name, std::vector<std::string>& args, std::string& reason)
    {
        ++statistic_.FailRunCounter;
        CommandInfo info;
        info.Name = name;
        info.Arguments = args;
        info.WasLaunched = false;
        info.WasTerminated  = false;
        info.WasKilled = false;
        info.Note = reason; //Is it true wrote "reason"?
        records_.push_back(info);
    }
    void RegisterForcedTermination()
    {
        ++statistic_.ForcedTerminationCounter;
        auto record = records_.back();
        record.WasKilled = true;
        record.WasTerminated = true;
    }
    void RegisterTermination()
    {
        ++statistic_.TerminationCounter;
        auto record = records_.back();
        record.WasTerminated = true;
    }
    void RegisterDownload(std::size_t bytes_received)
    {
        statistic_.DownloadedDataCounter += bytes_received;
    }
    void RegisterUpload(std::size_t bytes_transferred)
    {
        statistic_.UploadedDataCounter += bytes_transferred;
    }
    std::string Print()
    {       
        std::ostringstream os;
        os << "It is some information about connections.";
        //TODO: optimization!
        return os.str();
    }
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
        unsigned long long int RunCounter;
        unsigned long long int FailRunCounter;
        unsigned long long int TerminationCounter;
        unsigned long long int ForcedTerminationCounter;
        unsigned long long int DownloadedDataCounter;
        unsigned long long int UploadedDataCounter;
    };
    std::vector<CommandInfo> records_;
    Statistic statistic_;
    unsigned long long int connect_id_;
};
#endif
