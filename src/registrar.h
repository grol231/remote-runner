#ifndef __Registrar_H__
#define __Registrar_H_

struct Record;
struct Statistic;
class Registrar
{
public:
    Registrar():
        records_(),
        statistic_(std::make_unique<Statistic>())
    {}
    
    static void Write(const Record&){}
    static void Write(){};
    Registrar(const Registrar&) = delete;
    Registrar& operator=(const Registrar&) = delete;
private:
    std::vector<Record> records_;
    std::unique_ptr<Statistic> statistic_;
};
enum class Status {Start,Stop};
struct Record
{
    unsigned long long int ConnectID;
    std::string Command;
    std::string Condition;
    std::string Result;
    std::string Note;    
};
struct Statistic
{   
    Statistic():
        ConnectID(0),
        RunningCommandCounter(0),
        NotRunningCommandCounter(0),
        CompletedCommandCounter(0),
        CompletedCompulsorilyCommandCounter(0),
        DownloadedData(0),
        UploadedData(0)
    {}
    unsigned long long int ConnectID;
    unsigned long long int RunningCommandCounter;
    unsigned long long int NotRunningCommandCounter;
    unsigned long long int CompletedCommandCounter;
    unsigned long long int CompletedCompulsorilyCommandCounter;
    unsigned long long int DownloadedData;
    unsigned long long int UploadedData;
};
#endif
