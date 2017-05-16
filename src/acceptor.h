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
            /*(new Service(sock, m_allow_commands,
                         m_timeout, m_ios, m_connect_counter,m_log))
                ->StartHandling();//TOD: Use shared_ptr.
            //Service will become the  heir from enable_shared_from_this.
            */

        }
        else
        {
            std::cout << "Error occured! Error code = "
                << ec.value()
                << ". Message: " << ec.message();
        }
        if (!m_isStopped.load())
        {
            InitAccept();
        }
        else
        {
            m_acceptor.close();
        }
    }
private:
    boost::asio::io_service& m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::atomic<bool> m_isStopped;
    std::vector<std::string> m_allow_commands;
    boost::posix_time::seconds m_timeout;
    unsigned long long int m_connect_counter;
    src::severity_logger<logging::trivial::severity_level>& m_log;
};
#endif
