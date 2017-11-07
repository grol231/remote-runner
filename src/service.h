#ifndef __Service_H__
#define __Service_H__

#include <memory>
#include <string>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/error.hpp>
#include "config.h"
#include "log.h"

namespace Application
{
class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock,     
            boost::asio::io_service& ios,
            unsigned long long int connect_id,
            src::severity_logger<logging::trivial::severity_level>& log,
            bool logging);
    ~Service();
    void StartHandling(std::shared_ptr<Config> config);
private:    
    void OnRequestReceived(std::size_t bytes_transferred,
                           std::shared_ptr<Config> config);
    void OnResponseSent(const boost::system::error_code& ec,
                        std::size_t bytes_transferred);
    std::string ProcessRequest(std::shared_ptr<boost::asio::streambuf> buffer,
                               std::shared_ptr<Config> config);
    char** CreateArgv(const std::vector<std::string>& args) const;
    std::string ProcessError(int err);

    std::string response_;
    boost::asio::io_service& ios_;
    boost::asio::deadline_timer timer_;
    Logging::Statistic statistic_;
    src::severity_logger<logging::trivial::severity_level>& log_;
    bool logging_;
    std::function<void(void)> handling_;
    std::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    std::shared_ptr<boost::asio::streambuf> buffer_;
};
}
#endif
