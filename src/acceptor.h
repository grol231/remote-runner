#ifndef __Acceptor_H__
#define __Acceptor_H__

#include <iostream>
#include <memory>
#include <atomic>
#include <boost/asio.hpp>
#include "service.h"

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
        std::cout << "Acceptor::InitAccept" << std::endl;
        std::shared_ptr<boost::asio::ip::tcp::socket>
            sock(new boost::asio::ip::tcp::socket(ios_));
        acceptor_.async_accept(*sock.get(),
            [this, sock](const boost::system::error_code& error)
        {
            onAccept(error, sock);
        });
        std::cout << "End of Acceptor:InitAccept" << std::endl;
    }
    void onAccept(const boost::system::error_code& ec,
        std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        std::cout << "Acceptor:OnAccept" << std::endl;
        /*
        if (ec == 0)
        {
            std::shared_ptr<Service> service(std::make_shared<Service>(sock,ios_));
            service->StartHandling();
        }
        InitAccept();
        */
    }
private:
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
#endif
