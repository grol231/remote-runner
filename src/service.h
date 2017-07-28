#ifndef __Service_H__
#define __Service_H__

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include "runner.h"
#include "registrar.h"

class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock,     
        boost::asio::io_service& ios) :
            self_(shared_from_this()),
            sock_(sock),
            buffer_(std::make_shared<boost::asio::streambuf>()),                   
            ios_(ios),
            registrar_(std::make_shared<Registrar>()),
            runner_(std::make_shared<Runner>(ios_, registrar_))
    {
    }
    ~Service()
    {
        std::cout << "Service destroyed!" << std::endl;
    }
    void StartHandling()
    {/*
       std::weak_ptr<Service> weakSelf = shared_from_this();
       handling_ = [weakSelf](){
           std::shared_ptr<Service> self = weakSelf.lock();
           boost::asio::async_read_until(*self->sock_.get(),
               *self->buffer_.get(),
               '\n',
               [self](const boost::system::error_code& ec,
                      std::size_t bytes_transffered)
              { 
                         if(1 < bytes_transffered)
                         {                              
                             self->OnRequestReceived(bytes_transffered);
                         }
                         self->buffer_->consume(bytes_transffered);
                         self->handling_();
              }
           );
       };
       handling_();
       */
        std::cout << "Service:StartHandling" << std::endl;
        std::weak_ptr<Service> weakSelf = shared_from_this();
        boost::asio::async_read_until(*sock_.get(), *buffer_.get(),'\n', 
                [weakSelf](const boost::system::error_code& ec, long unsigned int bytes_transffered)
            {
                std::shared_ptr<Service> self = weakSelf.lock();
                if(1 < bytes_transffered)
                {
                    self->OnRequestReceived(bytes_transffered);
                }
                self->buffer_->consume(bytes_transffered);
                self.reset();
            });
    }
private:
    void OnRequestReceived(std::size_t bytes_transferred)
    {
        ProcessRequest(buffer_, bytes_transferred);
    }
    std::string ProcessRequest(std::shared_ptr<boost::asio::streambuf> buffer,
                               std::size_t bytes_transferred)
    {
        runner_->Execute();
        return "Don't worry be happy!";
    }
private:    
    std::shared_ptr<Service> self_;
    boost::asio::io_service& ios_;
    std::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    std::shared_ptr<boost::asio::streambuf> buffer_;
    std::shared_ptr<Runner> runner_;
    std::shared_ptr<Registrar> registrar_; 
};
#endif
