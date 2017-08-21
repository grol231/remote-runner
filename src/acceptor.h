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
            std::shared_ptr<Config> config)
        :ios_(ios),
        acceptor_(ios_,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4::any(),
                config->Port()
            )
        ),
        isStopped_(false),
        connect_counter_(0),
        log_(log),
        config_(config)
    {}
    ~Acceptor()
    {
    }
    void Start()
    {
        acceptor_.listen();
        InitAccept();
    }
    void Stop()
    {
        std::cout << "Stop" << std::endl;
        isStopped_.store(true);
    }
private:
    void InitAccept()
    {
        std::cout << "InitAccept" << std::endl;
        std::shared_ptr<boost::asio::ip::tcp::socket>
            sock(new boost::asio::ip::tcp::socket(ios_));
        acceptor_.async_accept(*sock.get(),
            [this, sock]( 
            const boost::system::error_code& error)
        {
            onAccept(error, sock);
        });
        ++connect_counter_;
    }
    void onAccept(const boost::system::error_code& ec,
        std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec == 0)
        {
            std::shared_ptr<Service> service(
                    new Service(sock, ios_, connect_counter_, log_));
            service->StartHandling(config_);
        }
        else
        {
            std::cout << "Error occured! Error code = "
                << ec.value()
                << ". Message: " << ec.message();
        }
        if (!isStopped_.load())
        {
            InitAccept();
        }
        else
        {
            acceptor_.close();
        }
    }
private:
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::atomic<bool> isStopped_;
    unsigned long long int connect_counter_;
    src::severity_logger<logging::trivial::severity_level>& log_;
    std::shared_ptr<Config> config_;
};
#endif
