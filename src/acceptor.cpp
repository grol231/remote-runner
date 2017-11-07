#include <iostream>
#include "acceptor.h"

using namespace std;
using namespace boost::asio;

namespace Application
{
Acceptor::Acceptor(io_service& ios,
                   src::severity_logger<logging::trivial::severity_level>& log,
                   shared_ptr<Config> config)
    :ios_(ios),
    acceptor_(ios_,
        ip::tcp::endpoint(
            ip::address_v4::any(),
            config->Port()
        )
    ),
    is_stopped_(false),
    connect_counter_(0),
    log_(log),
    config_(config)
{}
void Acceptor::Start()
{
    acceptor_.listen();
    InitAccept();
}
void Acceptor::Stop()
{
    is_stopped_.store(true);
}
void Acceptor::InitAccept()
{
    shared_ptr<ip::tcp::socket>
        sock(new ip::tcp::socket(ios_));
    acceptor_.async_accept(*sock.get(),
        [this, sock]( 
        const boost::system::error_code& error)
    {
        onAccept(error, sock);
    });
    ++connect_counter_;
}
void Acceptor::onAccept(const boost::system::error_code& ec,
                        std::shared_ptr<ip::tcp::socket> sock)
{
    if (ec == 0)
    {
        shared_ptr<Service> service(
                new Service(sock, ios_, connect_counter_, log_, config_->Logging()));
        service->StartHandling(config_);
    }
    else
    {
        cout << "Error occured! Error code = "
            << ec.value()
            << ". Message: " << ec.message();
    }
    if (!is_stopped_.load())
    {
        InitAccept();
    }
    else
    {
        acceptor_.close();
    }
}
}
