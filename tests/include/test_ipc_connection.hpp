#ifndef	TEST_IPC_CONNECTION_HPP
#define	TEST_IPC_CONNECTION_HPP

#include <functional>
#include <stdexcept>

#include "request.hpp"
#include "ipc_connection.hpp"
#include "response.hpp"

namespace server_uts {
	class TestIpcConnection: public ipc::IpcConnection {
	public:
		using SendAction = std::function<void(const server::Response&)>;
		TestIpcConnection(const SendAction& send_action): m_send_action(send_action) {
			if (!m_send_action) {
				throw std::invalid_argument("invalid action received");
			}
		}

		void set_callback(const Callback& cb) override {
			m_callback = cb;
		}

		void send(const server::Response& response) const override {
			m_send_action(response);
		}

		void publish_request(const server::Request& request) {
			if (!m_callback) {
				return;
			}
			m_callback(request);
		}
	private:
		SendAction m_send_action;
		unsigned int m_readable_timeout_ms;
		Callback m_callback;
	};
}

#endif // TEST_IPC_CONNECTION_HPP