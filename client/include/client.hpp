#ifndef	CLIENT_HPP
#define	CLIENT_HPP

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <string>

#include "connection.hpp"
#include "functional_listener.hpp"

namespace client {
	template <typename Traw_data, typename Tsubscriber_id>
	class Client {
	public:
		using ServerConnection = mcu_control::Connection<Tsubscriber_id, Traw_data>;
		Client(ServerConnection *connection, int server_response_timeout_ms);
		Client(const Client& other) = delete;
		Client& operator=(const Client& other) = delete;
		virtual ~Client() noexcept = default;

		Traw_data run(const Traw_data& data) const;
	private:
		ServerConnection *m_connection;
		int m_server_response_timeout_ms;

		mutable std::mutex m_mux;
		mutable std::condition_variable m_cond;
		Traw_data m_report;
	};

	template <typename Traw_data, typename Tsubscriber_id>
	inline Client<Traw_data, Tsubscriber_id>::Client(ServerConnection *connection, int server_response_timeout_ms): m_connection(connection), m_server_response_timeout_ms(server_response_timeout_ms) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	template <typename Traw_data, typename Tsubscriber_id>
	inline Traw_data Client<Traw_data, Tsubscriber_id>::run(const Traw_data& data) const {
		std::unique_lock lock(m_mux);
		auto sub_id = m_connection->subscribe(
			mcu_control_utl::FunctionalListener<const Traw_data&>(
				[this](const Traw_data& data) {
					std::unique_lock lock(m_mux);
					m_report = data;
					m_cond.notify_all();
					lock.unlock();
				}
			)
		);
		m_connection->send_data(data);
		if (std::cv_status::timeout == m_cond.wait_for(lock, std::chrono::milliseconds(m_server_response_timeout_ms))) {
			lock.unlock();
			throw std::runtime_error("timeout waiting for server response " + std::to_string(m_server_response_timeout_ms) + " ms exceeded");
		}
		return m_report;
	}
}

#endif // CLIENT_HPP