#ifndef __Server_H__
#define __Server_H__

#include <vector>
#include <thread>
#include <memory>
#include <boost/asio.hpp>
#include "acceptor.h"

class Server {
public:
	Server() {
		m_work.reset(new boost::asio::io_service::work(m_ios));
	}
	void Start(unsigned short port_num,	unsigned int thread_pool_size)
    {
		assert(thread_pool_size > 0);
		acc.reset(new Acceptor(m_ios, port_num));
		acc->Start();
		for (unsigned int i = 0; i < thread_pool_size; i++) {
			std::unique_ptr<std::thread> th(
                new std::thread([this]()
                {
                    m_ios.run();
                }));
			m_thread_pool.push_back(std::move(th));
		}
	}
	void Stop() {
		acc->Stop();
		m_ios.stop();
		for (auto& th : m_thread_pool) {
			th->join();
		}
	}

private:
	boost::asio::io_service m_ios;
	std::unique_ptr<boost::asio::io_service::work> m_work;
	std::unique_ptr<Acceptor> acc;
	std::vector<std::unique_ptr<std::thread>> m_thread_pool;
};
#endif
