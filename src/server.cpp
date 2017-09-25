#include "server.h"

Server::Server(bool logging):
    log_()
{
    Logging::InitializeLog(logging);
    work_.reset(new boost::asio::io_service::work(ios_));
}
Server::~Server()
{
}
void Server::Start(std::shared_ptr<Config> config,
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
void Server::Stop()
{
    acceptor_->Stop();
    ios_.stop();
    for (auto& th : thread_pool_)
    {
        th->join();
    }
}
