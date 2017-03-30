#ifndef __Acceptor_H__
#define __Acceptor_H__

#include <iostream>
#include <memory>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/asio.hpp>
#include "service.h"
#include "log.h"

class Acceptor
{
public:
    Acceptor(boost::asio::io_service& ios, unsigned short port_num,
        const std::vector<std::string>& allow_commands,
        boost::posix_time::seconds timeout,
        src::severity_logger<logging::trivial::severity_level>& log
        ) :
        m_ios(ios),
        m_acceptor(m_ios,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4::any(),
                port_num
            )
        ),
        m_isStopped(false),
        m_allow_commands(allow_commands),
        m_timeout(timeout),
        m_connect_counter(0),
        m_log(log)
    {}
    ~Acceptor()
    {
        std::cout << "Acceptor destroyed!" << std::endl;
    }
    void Start()
    {
        std::cout << "Start" << std::endl;
        m_acceptor.listen();
        InitAccept();
    }
    void Stop()
    {
        std::cout << "Stop" << std::endl;
        m_isStopped.store(true);
        //sock.shutdown(error::shutdawn_send)
    }
private:
    void InitAccept()
    {
        std::cout << "InitAccept" << std::endl;
        std::shared_ptr<boost::asio::ip::tcp::socket>
            sock(new boost::asio::ip::tcp::socket(m_ios));
        /*
        boost::asio::socket_base::keep_alive option;        
        sock->set_option(option);        
        struct timeval tv;
        unsigned int timeout_milli = 10000;
        tv.tv_sec = timeout_milli / 1000;
        tv.tv_usec = timeout_milli % 1000;        
        setsockopt(sock->native(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        setsockopt(sock->native(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
        */
        m_acceptor.async_accept(*sock.get(),
            [this, sock]( //TODO:Allow_commands must be a shared_ptr.
            const boost::system::error_code& error)
        {
            onAccept(error, sock);
        });
        ++m_connect_counter;
    }
    void onAccept(const boost::system::error_code& ec,
        std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        std::cout << "onAccept" << std::endl;
        if (ec == 0)
        {
            std::shared_ptr<Service> service(
                    new Service(sock));
           // std::function<void(void)> lambda = [&lambda, service]()
            unsigned long long int counter = 0;
            m_lambda = [this, service, &counter]()
                {       
                    /*
                     boost::asio::async_read_until(*service->Socket().get(),
                        *service->Buffer().get(),
                        '\n',
                        [&lambda,service](const boost::system::error_code& ec,
                            std::size_t bytes_transferred)
                            {
                                if(0 == ec)
                                {
                                    std::cout 
                                        << "Async operation success!" << std::endl;
                                    std::string buf;
                                    std::istream(service->Buffer().get()) >> buf;
                                    std::cout << buf << std::endl;
                                    //service->OnRequestReceived();
                                    lambda();
                                }
                                else
                                {
                                    std::cout << "Async operation error!" << std::endl;
                                }
                            }
                    );
                    */                   
                    m_lambda();
                };
           m_lambda();
        }
        else
        {
            std::cout << "Error occured! Error code = "
                << ec.value()
                << ". Message: " << ec.message();
        }   
        InitAccept();
    }
private:
    boost::asio::io_service& m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::atomic<bool> m_isStopped;
    std::vector<std::string> m_allow_commands;
    boost::posix_time::seconds m_timeout;
    unsigned long long int m_connect_counter;
    src::severity_logger<logging::trivial::severity_level>& m_log;
    std::function<void(void)> m_lambda; 
};
#endif
