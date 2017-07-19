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
    Acceptor(boost::asio::io_service& ios)
        :ios_(ios),
        acceptor_(ios_,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4::any(),
                12345
            )
        )
    {
    }
    void Start()
    {
        acceptor_.listen();
        InitAccept();
    }
private:
    void InitAccept()
    {
        std::shared_ptr<boost::asio::ip::tcp::socket>
            sock(new boost::asio::ip::tcp::socket(ios_));
        acceptor_.async_accept(*sock.get(),
            [this, sock](const boost::system::error_code& error)
        {
            onAccept(error, sock);
        });
    }
    void onAccept(const boost::system::error_code& ec,
        std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec == 0)
        {
           auto p = new Service(sock, ios_);
            std::cout << "Let's create a shared_ptr!" << std::endl;
            std::shared_ptr<Service> service(p);

            service->StartHandling();
        }
        InitAccept();
    }
private:
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
#endif
