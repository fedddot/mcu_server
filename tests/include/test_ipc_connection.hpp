#ifndef	TEST_IPC_CONNECTION_HPP
#define	TEST_IPC_CONNECTION_HPP

#include <functional>
#include <map>
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

		void subscribe(const std::string& id, const Callback& cb) override {
			if (is_subscribed(id)) {
				throw std::invalid_argument("received id is already subscribed");
			}
			m_callbacks.insert({id, cb});
		}

		void unsubscribe(const std::string& id) override {
			if (!is_subscribed(id)) {
				throw std::invalid_argument("received id is not subscribed");
			}
			const auto iter = m_callbacks.find(id);
			m_callbacks.erase(iter);
		}

		bool is_subscribed(const std::string& id) const override {
			return m_callbacks.end() == m_callbacks.find(id);
		}

		void send(const server::Response& response) const override {
			m_send_action(response);
		}

		void publish_request(const server::Request& request) {
			for (auto [id, cb]: m_callbacks) {
				cb(request);
			}
		}
	private:
		SendAction m_send_action;
		std::map<std::string, Callback> m_callbacks;
	};
}

#endif // TEST_IPC_CONNECTION_HPP