#ifndef	TEST_HTTP_CNC_SERVER_HPP
#define	TEST_HTTP_CNC_SERVER_HPP

#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "cnc_server.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "http_ipc_connection.hpp"
#include "object.hpp"
#include "server.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

namespace cnc_server {
	class TestHttpCncServer: public server::Server {
	public:
		TestHttpCncServer(const std::string& uri, const unsigned int polling_timeout_s, const unsigned int response_timeout_s);
		TestHttpCncServer(const TestHttpCncServer& other) = delete;
		TestHttpCncServer& operator=(const TestHttpCncServer& other) = delete;

		void run() override;
		bool is_running() const override;
		void stop() override;
	private:
		std::unique_ptr<ipc::HttpIpcConnection> m_connection;
		std::unique_ptr<server::Server> m_cnc_server;
	};

	
	inline TestHttpCncServer::TestHttpCncServer(const std::string& uri, const unsigned int polling_timeout_s, const unsigned int response_timeout_s):
		m_connection(new ipc::HttpIpcConnection(uri, polling_timeout_s, response_timeout_s)),
		m_cnc_server(
			new CncServer(
				m_connection.get(),
				m_connection.get(),
				[](const server::Data& data)-> manager::Gpio * {
					using namespace server;
					using namespace manager;
					const auto cfg = Data::cast<Object>(data);
					const auto dir = static_cast<Gpio::Direction>(Data::cast<Integer>(cfg.access("direction")).get());
					switch (dir) {
					case Gpio::Direction::IN:
						return new manager_uts::TestGpi();
					case Gpio::Direction::OUT:
						return new manager_uts::TestGpo();
					default:
						throw std::invalid_argument("unsupported gpio type");
					}
				},
				[](const unsigned int delay) {
					std::this_thread::sleep_for(std::chrono::milliseconds(delay));
				}
			)
		) {
	}

	
	inline void TestHttpCncServer::run() {
		m_cnc_server->run();
	}

	
	inline bool TestHttpCncServer::is_running() const {
		return m_cnc_server->is_running();
	}

	
	inline void TestHttpCncServer::stop() {
		m_cnc_server->stop();
	}
}

#endif // TEST_HTTP_CNC_SERVER_HPP