#ifndef	TEST_IPC_CONNECTION_HPP
#define	TEST_IPC_CONNECTION_HPP

#include <functional>
#include <map>
#include <stdexcept>

#include "request.hpp"
#include "ipc_connection.hpp"
#include "response.hpp"

namespace server_uts {
	
	template <typename Tsubscriber_id>
	class TestIpcConnection: public ipc::IpcConnection<Tsubscriber_id, server::Request, server::Response> {
	public:
		using Callback = typename ipc::IpcConnection<Tsubscriber_id, server::Request, server::Response>;
		using SendAction = std::function<void(const server::Response&)>;

		TestIpcConnection(const SendAction& send_action): m_send_action(send_action) {
			if (!m_send_action) {
				throw std::invalid_argument("invalid action received");
			}
		}

		void subscribe(const Tsubscriber_id& id, const Callback& cb) override {
			if (is_subscribed(id)) {
				throw std::invalid_argument("received id is already subscribed");
			}
			m_callbacks.insert({id, cb});
		}

		void unsubscribe(const Tsubscriber_id& id) override {
			if (!is_subscribed(id)) {
				throw std::invalid_argument("received id is not subscribed");
			}
			const auto iter = m_callbacks.find(id);
			m_callbacks.erase(iter);
		}

		bool is_subscribed(const Tsubscriber_id& id) const override {
			return m_callbacks.end() != m_callbacks.find(id);
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
		std::map<Tsubscriber_id, Callback> m_callbacks;
	};
}

#endif // TEST_IPC_CONNECTION_HPP