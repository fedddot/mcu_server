#ifndef	CLIENT_HPP
#define	CLIENT_HPP

#include <stdexcept>
#include "connection.hpp"

namespace client {
	template <typename Traw_data>
	class Client {
	public:
		using ServerConnection = mcu_control::Connection<Traw_data>;
		Client(ServerConnection *connection);
		Client(const Client& other) = delete;
		Client& operator=(const Client& other) = delete;
		virtual ~Client() noexcept = default;

		Traw_data run(const Traw_data& data) const;
	private:
		ServerConnection *m_connection;
	};

	template <typename Traw_data>
	inline Client<Traw_data>::Client(ServerConnection *connection): m_connection(connection) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	template <typename Traw_data>
	inline Traw_data Client<Traw_data>::run(const Traw_data& data) const {
		m_connection->send_data(data);
		while (!m_connection->is_readable()) {
			;
		}
		return m_connection->read_data();
	}
}

#endif // CLIENT_HPP