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
            unsigned short port_num,
            const std::vector<std::string>& allow_commands,
            boost::posix_time::seconds timeout,
            src::severity_logger<logging::trivial::severity_level>& log)
        :ios_(ios),
        acceptor_(ios_,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4::any(),
                port_num
            )
        ),
        isStopped_(false),
        allow_commands_(allow_commands),
        timeout_(timeout),
        connect_counter_(0),
        log_(log)
    {}
    ~Acceptor()
    {
        std::cout << "Acceptor destroyed!" << std::endl;
    }
    void Start()
    {
        std::cout << "Start" << std::endl;
        acceptor_.listen();
        InitAccept();
    }
    void Stop()
    {
        std::cout << "Stop" << std::endl;
        isStopped_.store(true);
        //sock.shutdown(error::shutdawn_send)
    }
private:
    void InitAccept()
    {
        std::cout << "InitAccept" << std::endl;
        std::shared_ptr<boost::asio::ip::tcp::socket>
            sock(new boost::asio::ip::tcp::socket(ios_));
        acceptor_.async_accept(*sock.get(),
            [this, sock]( //TODO:Allow_commands must be a shared_ptr.
            const boost::system::error_code& error)
        {
            onAccept(error, sock);
        });
        ++connect_counter_;
    }
    void onAccept(const boost::system::error_code& ec,
        std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        std::cout << "onAccept" << std::endl;
        if (ec == 0)
        {
            std::shared_ptr<Service> service(
                    new Service(sock, allow_commands_, timeout_,
                                ios_, connect_counter_, log_));
            service->StartHandling();

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
    std::vector<std::string> allow_commands_;
    boost::posix_time::seconds timeout_;
    unsigned long long int connect_counter_;
    src::severity_logger<logging::trivial::severity_level>& log_;
};
#endif
