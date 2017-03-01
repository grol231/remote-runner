#ifndef __Service_H__
#define __Service_H__

#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <boost/asio.hpp>

class Service
{
public:
	Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) :
		m_sock(sock)
	{}
	void StartHandling()
	{
		boost::asio::async_read_until(*m_sock.get(),
			m_request,
			'\n',
			[this](const boost::system::error_code& ec,
                std::size_t bytes_transferred)
            {
                onRequestReceived(ec,bytes_transferred);
            }
        );
	}
private:
	void onRequestReceived(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
    {
		if (ec != 0)
		{
			std::cout << "Error occured! Error code = "
				<< ec.value()
				<< ". Message: " << ec.message();
			onFinish();
			return;
		}
		std::string request;
		std::istream(&m_request) >> request;
		//std::istream(&request) >> data;
		std::cout << "Request:" << request << std::endl;
		m_response = ProcessRequest(m_request);
		boost::asio::async_write(*m_sock.get(),
            boost::asio::buffer(m_response),
			[this](const boost::system::error_code& ec,
                std::size_t bytes_transferred)
            {
                onResponseSent(ec, bytes_transferred);
            }
        );
	}
	void onResponseSent(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
    {
		if (ec != 0)
		{
			std::cout << "Error occured! Error code = "
				<< ec.value()
				<< ". Message: " << ec.message();
		}
		boost::asio::async_read_until(*m_sock.get(),
			m_request,
			'\n',
			[this](const boost::system::error_code& ec,
                std::size_t bytes_transferred)
            {
                onRequestReceived(ec, bytes_transferred);
            }
        );
//        //onFinish();
	}
	void onFinish()
	{
		delete this;
	}
	std::string ProcessRequest(boost::asio::streambuf& request)
	{
        std::string data;
		std::istream(&request) >> data;
		std::string response = "Response";
		pid_t pid = fork();//todo: error handling
		int err(0);
		if(!pid){
            err = execlp(data.c_str(),NULL);
            response += "-child:";
		}else{
            response += "-parent:";
		}

        if(-1 == err)
        {
            switch(errno)
            {
            case E2BIG:
                response += "E2BIG";
                break;
            case EACCES:
                response += "EACCES";
                break;
            case EFAULT:
                response += "EFAULT";
                break;
            case EINVAL:
                response += "EINVAL";
                break;
            case EIO:
                response += "EIO";
                break;
            case EISDIR:
                response += "EISDIR";
                break;
            case ELIBBAD:
                response += "ELIBBAD";
                break;
            case ELOOP:
                response += "ELOOP";
                break;
            case EMFILE:
                response += "EMFILE";
                break;
            case ENAMETOOLONG:
                response += "ENAMETOOLONG";
                break;
            case ENFILE:
                response += "ENFILE";
                break;
            case ENOENT:
                response += "ENOENT";
                break;
            case ENOEXEC:
                response += "ENOEXEC";
                break;
            case ENOMEM:
                response += "ENOMEM";
                break;
            case ENOTDIR:
                response += "ENOTDIR";
                break;
            case EPERM:
                response += "EPERM";
                break;
            case ETXTBSY:
                response += "ETXTBSY";
                break;
            default:
                response += "Uknown error code.";
            }
        }
        else
        {
            response += "success";
        }
        response += "\n";
		return response;
	}
private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
	std::string m_response;
	boost::asio::streambuf m_request;
};
#endif
