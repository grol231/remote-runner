#include <boost/bind.hpp>
#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/asio/read.hpp>
#include "server.h"

using namespace boost::asio;
using namespace boost::system;
using namespace std;

class server
{
public:
    using socket_ptr = boost::shared_ptr<ip::tcp::socket>;
    server():io_service_()
    {}
    void client_session(){
		printf("Hello world!\n");
	}
	size_t read_complete(char* buff, const boost::system::error_code& err, size_t bytes){
        if(err)
        {
            std::cout << "Game over!" << std::endl;
            return 0;
        }
        else
        {
            std::cout << "I am in read_complete." << std::endl;
        }
        bool found = std::find(buff, buff+bytes,'\n') < buff+bytes;
        return found ? 0 : 1;
	}
    void run()
    {
        std::cout << "Server run!" << std::endl;
        ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
        ip::tcp::acceptor acc(this->io_service_, ep);
        size_t size = 1024;
        char data[size];
        while(true)
        {
            server::socket_ptr sock(new ip::tcp::socket(io_service_));
            acc.accept(*sock);
            boost::system::error_code e;
            size_t bytes = boost::asio::read(*sock, boost::asio::buffer(data, size),
            boost::bind(&server::read_complete,this,data,_1,_2));
            std::string msg(data, bytes);
            std::cout << msg << std::endl;
            sock->close();
            cout << "Socket terminate!" << endl;
        }
        cout << "Server terminate!" << endl;
    }
private:
   boost::asio::io_service io_service_;
};

int main()
{
	server s;
	s.run();
	return 0;
}



//namespace
//{
//  void overload(int param1, float param2, int param3) {}
//  void overload(int param) {}
//  void overload(float param) {}
//
//  class Class
//  {
//  public: // interface
//    void overload(float param) {}
//    void overload(int param) {}
//  };
//
//} // namespace
//
//int main(int arg, char** argv)
//{
//  // non-member
//  boost::bind(::overload, _1, _2, _3); // can bind normally
//  typedef void (*NonMemberFuncType)(int);
//  boost::bind(static_cast<NonMemberFuncType>(::overload), _1);
//
//  // member
//  Class* objPtr = new Class();
//  typedef void (Class::*OverloadFuncType)(float);
//  boost::bind(static_cast<OverloadFuncType>(&Class::overload), objPtr, _1);
//
//} // main
