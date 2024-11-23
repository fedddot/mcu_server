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
#include "test_gpi.hpp"
#include "test_gpo.hpp"

namespace cnc_server {
	template <typename Tsubscriber_id>
	class TestHttpCncServer: public CncServer<Tsubscriber_id> {
	public:
		TestHttpCncServer(const Tsubscriber_id& id, const std::string& uri, const unsigned int timeout_s);
		TestHttpCncServer(const TestHttpCncServer& other) = delete;
		TestHttpCncServer& operator=(const TestHttpCncServer& other) = delete;
	private:
		using IpcConnection = typename CncServer<Tsubscriber_id>::IpcConnection;
		std::unique_ptr<IpcConnection> m_connection;
		IpcConnection *init_connection(const std::string& uri, const unsigned int timeout_s);
	};

	template <typename Tsubscriber_id>
	inline TestHttpCncServer<Tsubscriber_id>::TestHttpCncServer(const Tsubscriber_id& id, const std::string& uri, const unsigned int timeout_s):
		CncServer<Tsubscriber_id>(
			init_connection(uri, timeout_s),
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
		) {

	}

	template <typename Tsubscriber_id>
	typename TestHttpCncServer<Tsubscriber_id>::IpcConnection  *TestHttpCncServer<Tsubscriber_id>::init_connection(const std::string& uri, const unsigned int timeout_s) {
		m_connection = std::unique_ptr<IpcConnection>(
			new server_utl::HttpIpcConnection<Tsubscriber_id>(uri, timeout_s)
		);
		return m_connection.get();
	}
}

#endif // TEST_HTTP_CNC_SERVER_HPP