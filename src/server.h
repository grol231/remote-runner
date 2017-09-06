#ifndef __Server_H__
#define __Server_H__

#include <vector>
#include <thread>
#include <memory>
#include <boost/asio.hpp>
#include "acceptor.h"
#include "log.h"

class Server
{
public:
    Server():
        log_()
    {
        Logging::InitializeLog();
        work_.reset(new boost::asio::io_service::work(ios_));
    }
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server operator=(const Server&&) = delete;
    ~Server()
    {
    }
    void Start(std::shared_ptr<Config> config,
        unsigned int thread_pool_size)
    {
        assert(thread_pool_size > 0);
        acceptor_.reset(new Acceptor(ios_, log_, config));
        acceptor_->Start();
        for (unsigned int i = 0; i < thread_pool_size; i++)
        {
            std::unique_ptr<std::thread> th(
                new std::thread([this](){ios_.run();})
            );
            thread_pool_.push_back(std::move(th));
        }
    }
    void Stop()
    {
        acceptor_->Stop();
        ios_.stop();
        for (auto& th : thread_pool_)
        {
            th->join();
        }
    }
private:
    boost::asio::io_service ios_;
    std::unique_ptr<boost::asio::io_service::work> work_;
    std::unique_ptr<Acceptor> acceptor_;
    std::vector<std::unique_ptr<std::thread>> thread_pool_;
    src::severity_logger<logging::trivial::severity_level> log_;
};
#endif
