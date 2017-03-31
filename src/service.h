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
        std::string response = ProcessRequest();
        boost::asio::async_write(*sock_.get(),
                boost::asio::buffer(response),
                [this, response](const boost::system::error_code& ec, 
                    std::size_t bytes_transffered)
                {
                    OnResponseSent(response);
                }
        );
    }
    void StartHandling()
    {      
        std::weak_ptr<Service> weakSelf = shared_from_this();
        handling_ = [weakSelf]()
            {       
                std::shared_ptr<Service> self = weakSelf.lock();
                boost::asio::async_read_until(*self->Socket().get(),
                    *self->Buffer().get(),
                    '\n',
                    [weakSelf](const boost::system::error_code& ec,
                        std::size_t bytes_transffered)
                    {
                        std::shared_ptr<Service> self = weakSelf.lock();
                        if(0 == ec)
                        {
                            if(1 < bytes_transffered)//This is crutch!
                            {
                                std::cout 
                                    << "Async operation success!" << std::endl;                                    
                                self->OnRequestReceived();
                            }
                            self->buffer_->consume(bytes_transffered);
                            self->handling_();
                        }
                        else
                        {
                            std::cout << "Async operation error!" << std::endl;
                        }
                    }
                );
            };
        handling_();
    }
    
    std::string ProcessRequest(){return std::string();}
    void OnResponseSent(const std::string&){}
    std::shared_ptr<boost::asio::ip::tcp::socket> Socket(){return sock_;}
    std::shared_ptr<boost::asio::streambuf> Buffer(){return buffer_;}
private:
    std::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    std::shared_ptr<boost::asio::streambuf> buffer_;
    std::function<void(void)> handling_; 
};
#endif

