//#include <boost/bind.hpp>
//#include <stdio.h>
//#include <boost/asio.hpp>
//#include <boost/asio/read.hpp>
//#include "server.h"
//#include <unistd.h>
////#include <sys/types>
//
//using namespace boost::asio;
//using namespace boost::system;
//using namespace std;
//
//class server
//{
//public:
//    using socket_ptr = boost::shared_ptr<ip::tcp::socket>;
//    server():io_service_()
//    {}
//    void client_session(){
//		printf("Hello world!\n");
//	}
//	size_t read_complete(char* buff, const boost::system::error_code& err, size_t bytes){
//        if(err)
//        {
//            std::cout << "Game over!" << std::endl;
//            return 0;
//        }
//        else
//        {
//            //std::cout << "I am in read_complete." << std::endl;
//        }
//        bool found = std::find(buff, buff+bytes,'\n') < buff+bytes;
//        return found ? 0 : 1;
//	}
//    void run()
//    {
//        std::cout << "Server run!" << std::endl;
//        ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
//        ip::tcp::acceptor acc(this->io_service_, ep);
//        size_t size = 1024;
//        char data[size];
//        server::socket_ptr sock(new ip::tcp::socket(io_service_));
//        acc.accept(*sock);
//        while(true)
//        {
//            //server::socket_ptr sock(new ip::tcp::socket(io_service_));
//            //acc.accept(*sock);
//            size_t bytes = boost::asio::read(*sock, boost::asio::buffer(data, size),
//                boost::bind(&server::read_complete,this,data,_1,_2));
//            std::string msg(data, bytes);
//            std::cout << msg << std::endl;
//            //sock->close();
//            //cout << "Socket terminate!" << endl;
//            if(5 == msg.size()){
//                cout << "Exit!" << endl;
//                break;
//            }
//            msg.pop_back();
//
//        }
//        cout << "Server terminate!" << endl;
//    }
//private:
//   boost::asio::io_service io_service_;
//};
#include <stdio.h>
#include <unistd.h>
int main()
{
    while(true)
    {
        if(fork())
            execl("main",NULL);
    }
}


