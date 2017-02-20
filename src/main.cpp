#include <boost/bind.hpp>
#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/asio/read.hpp>
#include "server.h"
#include <unistd.h>


using namespace boost::asio;
using namespace boost::system;
using namespace std;

class server
{
public:
    using socket_ptr = boost::shared_ptr<ip::tcp::socket>;
    server():io_service_()
    {}

    void run()
    {
        cout << "Server run!" << endl;
        ip::tcp::acceptor acceptor(io_service_, ip::tcp::endpoint(ip::tcp::v4(), 2001));
        ip::tcp::iostream stream;
        acceptor.accept(*stream.rdbuf());
        cout << "Request : " << stream.rdbuf();
        stream << "Response";
    }
private:
   boost::asio::io_service io_service_;
};

int main()
{
    server s;
    s.run();
}


