#include <boost/bind.hpp>
#include <stdio.h>
#include <boost/asio.hpp>
#include "server.h"

using namespace boost::asio;

class server
{
public:
    using socket_ptr = boost::shared_ptr<ip::tcp::socket>;
    server():io_service_()
    {}
    void client_session(){
		printf("Hello world!\n");
	}
    void run()
    {
        ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
        ip::tcp::acceptor acc(this->io_service_, ep);
        size_t size = 1024;
        char data[size];
        while(true)
        {
            server::socket_ptr sock(new ip::tcp::socket(io_service_));
            acc.accept(*sock);
            boost::system::error_code e;
            size_t result = boost::asio::read(sock, boost::asio::buffer(data, size), e);
            //int c = boost::asio::read(sock, data, boost::asio::buffer(data),boost::bind(&server::client_session,this) );
        }
    }



private:
   boost::asio::io_service io_service_;
};

int main()
{
	//server s;
	//s.run();
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
