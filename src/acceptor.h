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
	Acceptor(boost::asio::io_service& ios, unsigned short port_num) :
		m_ios(ios),
		m_acceptor(m_ios,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4::any(),
                port_num
            )
        ),
		m_isStopped(false)
	{}
	void Start()
	{
		m_acceptor.listen();
		InitAccept();
	}
	void Stop()
	{
		m_isStopped.store(true);
	}
private:
	void InitAccept()
	{
		std::shared_ptr<boost::asio::ip::tcp::socket>
			sock(new boost::asio::ip::tcp::socket(m_ios));
		m_acceptor.async_accept(*sock.get(),
			[this, sock](
			const boost::system::error_code& error)
		{
			onAccept(error, sock);
		});
	}
	void onAccept(const boost::system::error_code& ec,
		std::shared_ptr<boost::asio::ip::tcp::socket> sock)
	{
		if (ec == 0)
		{
			(new Service(sock))->StartHandling();
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
};
#endif
