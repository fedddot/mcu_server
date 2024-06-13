#ifndef	CLIENT_HPP
#define	CLIENT_HPP

#include <stdexcept>
#include "connection.hpp"

namespace client {
	template <typename Traw_data, typename Ttime>
	class Client {
	public:
		using ServerConnection = mcu_control::Connection<Traw_data, Ttime>;
		Client(ServerConnection *connection, const Ttime& server_response_timeout);
		Client(const Client& other) = delete;
		Client& operator=(const Client& other) = delete;
		virtual ~Client() noexcept = default;

		Traw_data run(const Traw_data& data) const;
	private:
		ServerConnection *m_connection;
		Ttime m_server_response_timeout;
	};

	template <typename Traw_data, typename Ttime>
	inline Client<Traw_data, Ttime>::Client(ServerConnection *connection, const Ttime& server_response_timeout): m_connection(connection), m_server_response_timeout(server_response_timeout) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	template <typename Traw_data, typename Ttime>
	inline Traw_data Client<Traw_data, Ttime>::run(const Traw_data& data) const {
		m_connection->send_data(data);
		if (!m_connection->poll_data(m_server_response_timeout)) {
			throw std::runtime_error("timeout waiting for server response exceeded");
		}
		return m_connection->read_data();
	}
}

#endif // CLIENT_HPP