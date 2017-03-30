#ifndef __Service_H__
#define __Service_H__

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/error.hpp>
#include "log.h"

class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock):
        sock_(sock),
        buffer_(std::make_shared<boost::asio::streambuf>())
    {        
    }
    ~Service()
    {
        std::cout << "Service destroyed!" << std::endl;
    }
    void OnRequestReceived()
    {
        std::string response=ProcessRequest();
        boost::asio::async_write(*sock_.get(),
                boost::asio::buffer(response),
                [this, response](const boost::system::error_code& ec, std::size_t bytes_transffered)
                {
                    onResponseSent(response);
                }
        );
    }
    
    std::string ProcessRequest(){return std::string();}
    void onResponseSent(std::string&){}
    std::shared_ptr<boost::asio::ip::tcp::socket> Socket(){return sock_;}
    std::shared_ptr<boost::asio::streambuf> Buffer(){return buffer_;}
private:
    std::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    std::shared_ptr<boost::asio::streambuf> buffer_;
};
#endif

