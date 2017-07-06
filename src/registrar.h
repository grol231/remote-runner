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
        statistic_.Launches = 0;
        statistic_.FailedLaunches = 0;
        statistic_.Terminations = 0;
        statistic_.ForcedTerminations = 0;
        statistic_.DownloadedBytes = 0;
        statistic_.UploadedBytes = 0;
    }
    Registrar(const Registrar&) = delete;
    Registrar& operator=(const Registrar&) = delete;
    Registrar(Registrar&&) = delete;
    Registrar&& operator=(Registrar&&) = delete;

    void RegisterLaunch(std::string& name, std::vector<std::string>& args)
    {
        ++statistic_.Launches;
        CommandInfo info;
        info.Name = name;
        info.Arguments = args;
        info.WasLaunched = true;
        info.WasKilled = false;
        //TODO: optimization! use std::move()!
        records_.push_back(info);
    }
    void RegisterFailedLaunch(std::string& name, std::vector<std::string>& args, std::string& reason)
    {
        ++statistic_.FailedLaunches;
        CommandInfo info;
        info.Name = name;
        info.Arguments = args;
        info.WasLaunched = false;
        info.WasKilled = false;
        info.Note = reason; //Is it true wrote "reason"?
        records_.push_back(info);
    }
    void RegisterForcedTermination()
    {
        ++statistic_.ForcedTerminations;
        auto record = records_.back();
        record.WasKilled = true;
    }
    void RegisterTermination()
    {
        ++statistic_.Terminations;
        auto record = records_.back();
    }
    void RegisterDownload(std::size_t bytes_received)
    {
        statistic_.DownloadedBytes += bytes_received;
    }
    void RegisterUpload(std::size_t bytes_transferred)
    {
        statistic_.UploadedBytes += bytes_transferred;
    }
    std::string Print()
    {       
        std::ostringstream os;

        os << "ConnectId #" << connect_id_ << std::endl;
        os << "Statistic" << std::endl;
        os << "Launches: " << statistic_.Launches  << std::endl;
        os << "Failed launches: " << statistic_.FailedLaunches << std::endl;
        os << "Terminations: " << statistic_.Terminations << std::endl;
        os << "Forced Terminations: " << statistic_.ForcedTerminations << std::endl;
        os << "Downloaded bytes: " << statistic_.DownloadedBytes << std::endl;
        os << "Uploaded bytes: " << statistic_.UploadedBytes << std::endl;
        os << std::endl;
        for(const auto& record : records_)
        {
            os << "Command:" << record.Name << std::endl;
            os << "Arguments:";
            for(const auto& arg : record.Arguments)
            {
                os << arg << " ";
            }
            os << std::endl;
            if(!record.WasLaunched)
            {
                os << "Program didn't launch. Error:" << record.Note << std::endl;
            }
            else
            {
                os << "The program was launched." << std::endl;
                if(record.WasKilled)
                {
                    os << "The program was killed by timer." << std::endl;
                }
                else
                {
                    os << "The program terminated itself." << std::endl;
                }
            }
        }
        //TODO: use std::move
        return os.str();
    }
private:
    struct CommandInfo
    {   
        std::string Name;
        std::vector<std::string> Arguments;
        bool WasLaunched;
        bool WasKilled;
        std::string Note;    
    };
    struct Statistic
    {   
        unsigned long long int Launches;
        unsigned long long int FailedLaunches;
        unsigned long long int Terminations;
        unsigned long long int ForcedTerminations;
        unsigned long long int DownloadedBytes;
        unsigned long long int UploadedBytes;
    };
    std::vector<CommandInfo> records_;
    Statistic statistic_;
    unsigned long long int connect_id_;
};
#endif
