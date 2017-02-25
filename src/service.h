#ifndef __Service_H__
#define __Service_H__

#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <boost/asio.hpp>

class Service {
public:
	Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) :
		m_sock(sock)
	{}

	void StartHandling() {

		boost::asio::async_read_until(*m_sock.get(),
			m_request,
			'\n',
			[this](
			const boost::system::error_code& ec,
			std::size_t bytes_transferred)
		{
			onRequestReceived(ec,
				bytes_transferred);
		});
	}

private:
	void onRequestReceived(const boost::system::error_code& ec,
		std::size_t bytes_transferred) {
		if (ec != 0) {
			std::cout << "Error occured! Error code = "
				<< ec.value()
				<< ". Message: " << ec.message();

			onFinish();
			return;
		}

		// Process the request.
		m_response = ProcessRequest(m_request);

		// Initiate asynchronous write operation.
		boost::asio::async_write(*m_sock.get(),
                boost::asio::buffer(m_response),
			[this](
			const boost::system::error_code& ec,
			std::size_t bytes_transferred)
		{
			onResponseSent(ec,
				bytes_transferred);
		});
	}

	void onResponseSent(const boost::system::error_code& ec,
		std::size_t bytes_transferred) {
		if (ec != 0) {
			std::cout << "Error occured! Error code = "
				<< ec.value()
				<< ". Message: " << ec.message();
		}

		onFinish();
	}

	// Here we perform the cleanup.
	void onFinish() {
		delete this;
	}

	std::string ProcessRequest(boost::asio::streambuf& request) {

		// In this method we parse the request, process it
		// and prepare the request.

		// Emulate CPU-consuming operations.
		int i = 0;
		while (i != 1000000)
			i++;

		// Emulate operations that block the thread
		// (e.g. synch I/O operations).
		std::this_thread::sleep_for(
			std::chrono::milliseconds(100));

		// Prepare and return the response message.
		std::string response = "Response\n";
		return response;
	}

private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
	std::string m_response;
	boost::asio::streambuf m_request;
};
#endif
