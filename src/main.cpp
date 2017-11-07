#include <iostream>
#include <memory>
#include <exception>
#include <boost/asio/error.hpp>
#include "config.h"
#include "application.h"

using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        shared_ptr<Config> config(new Config(argc, argv));
        Application::Initialize(config);
    }
    catch(boost::system::system_error& e)
    {
        cout << "Error occured! Error code = "
			<< e.code() << ". Message: "
			<< e.what() << endl;
    }
    catch(exception& e)
    {
        cout << "Error occured!"
        ". Message: "<< e.what() << endl;
    }
    catch(...)
    {
        cout << "Unkown error occured!" << endl;
    }
	return 0;
}
