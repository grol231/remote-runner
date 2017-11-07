#ifndef __Server_H__
#define __Server_H__

#include <vector>
#include <thread>
#include <memory>
#include <boost/asio.hpp>
#include "acceptor.h"
#include "log.h"

namespace Application
{
class Server
{
public:
    Server(bool logging);
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server operator=(const Server&&) = delete;
    void Start(std::shared_ptr<Config> config, unsigned int thread_pool_size);
    void Stop();
private:
    boost::asio::io_service ios_;
    std::unique_ptr<boost::asio::io_service::work> work_;
    std::unique_ptr<Acceptor> acceptor_;
    std::vector<std::unique_ptr<std::thread>> thread_pool_;
    src::severity_logger<logging::trivial::severity_level> log_;
};
}
#endif
