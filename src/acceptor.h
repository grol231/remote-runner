#ifndef __Acceptor_H__
#define __Acceptor_H__

#include <iostream>
#include <memory>
#include <atomic>
#include <boost/asio.hpp>
#include "service.h"
#include "log.h"

class Acceptor
{
public:
    Acceptor(boost::asio::io_service& ios,
             src::severity_logger<logging::trivial::severity_level>& log,
             std::shared_ptr<Config> config);
    ~Acceptor();
    void Start();
    void Stop();
private:
    void InitAccept();
    void onAccept(const boost::system::error_code& ec,
                  std::shared_ptr<boost::asio::ip::tcp::socket> sock);

    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::atomic<bool> isStopped_;
    unsigned long long int connect_counter_;
    src::severity_logger<logging::trivial::severity_level>& log_;
    std::shared_ptr<Config> config_;
};
#endif
