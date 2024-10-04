#ifndef	TEST_IPC_CONNECTION_HPP
#define	TEST_IPC_CONNECTION_HPP

#include <functional>
#include <queue>
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

		bool readable() const override {
			return !m_requests_queue.empty();
		}

		server::Request read() override {
			if (!readable()) {
				throw std::runtime_error("connection is not readable");
			}
			auto res(m_requests_queue.front());
			m_requests_queue.pop();
			return res;
		}
		
		void send(const server::Response& response) const override {
			m_send_action(response);
		}

		void enqueue_request(const server::Request& request) {
			m_requests_queue.push(request);
		}
	private:
		SendAction m_send_action;
		std::queue<server::Request> m_requests_queue;
	};
}

#endif // TEST_IPC_CONNECTION_HPP