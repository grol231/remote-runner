#include "server.h"

using namespace std;

namespace Application
{
Server::Server(bool logging):
    log_()
{
    Logging::InitializeLog(logging);
    work_.reset(new boost::asio::io_service::work(ios_));
}
void Server::Start(shared_ptr<Config> config,
                   unsigned int thread_pool_size)
{
    assert(thread_pool_size > 0);
    acceptor_.reset(new Acceptor(ios_, log_, config));
    acceptor_->Start();
    for (unsigned int i = 0; i < thread_pool_size; i++)
    {
        unique_ptr<thread> th(
            new thread([this](){ios_.run();})
        );
        thread_pool_.push_back(move(th));
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
}
