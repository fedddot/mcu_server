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
#include "ipc_connection.hpp"
#include "object.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

namespace cnc_server {
	template <typename Tsubscriber_id>
	class TestHttpCncServer: public server::Server {
	public:
		TestHttpCncServer(const Tsubscriber_id& id, const std::string& uri, const unsigned int timeout_s);
		TestHttpCncServer(const TestHttpCncServer& other) = delete;
		TestHttpCncServer& operator=(const TestHttpCncServer& other) = delete;

		void run() override;
		bool is_running() const override;
		void stop() override;
	private:
		using IpcConnection = ipc::IpcConnection<Tsubscriber_id, server::Request, server::Response>;
		std::unique_ptr<IpcConnection> m_connection;
		std::unique_ptr<server::Server> m_cnc_server;
	};

	template <typename Tsubscriber_id>
	inline TestHttpCncServer<Tsubscriber_id>::TestHttpCncServer(const Tsubscriber_id& id, const std::string& uri, const unsigned int timeout_s):
		m_connection(new server_utl::HttpIpcConnection<Tsubscriber_id>(uri, timeout_s)),
		m_cnc_server(
			new CncServer<Tsubscriber_id>(
				m_connection.get(),
				id,
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

	template <typename Tsubscriber_id>
	inline void TestHttpCncServer<Tsubscriber_id>::run() {
		m_cnc_server->run();
	}

	template <typename Tsubscriber_id>
	inline bool TestHttpCncServer<Tsubscriber_id>::is_running() const {
		return m_cnc_server->is_running();
	}

	template <typename Tsubscriber_id>
	inline void TestHttpCncServer<Tsubscriber_id>::stop() {
		m_cnc_server->stop();
	}
}

#endif // TEST_HTTP_CNC_SERVER_HPP