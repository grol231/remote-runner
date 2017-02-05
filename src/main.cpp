#include <stdio.h>
#include <boost/asio.hpp>

class server
{
public:
    using boost::shared_ptr<ip::tcp::socket> socket_ptr;
    server():io_service_()
    {}
    void run()
    {
        ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
        ip::tcp::acceptor acc(io_service_, ep);
        char data[1024];
        while(true)
        {
            soket_ptr sock(new ip::tcp::socket(io_service_));
            acc.accept(*sock);
            //boost::thread( boost::bind(client_session, sock));
            int c = boost::asio::read(
            sock, data, boost::asio::buffer(data),boost::bind() );
        }
            
    }
    //void client_session()
private:
   boost::asio::io_service io_service_; 
};

int main()
{
	
	return 0;
}
