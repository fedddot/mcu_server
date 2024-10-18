#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <stdexcept>

#include "ipc_connection.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "string.hpp"
#include "vendor.hpp"

namespace server {
	template <typename Tsubscriber_id>
	class Server {
	public:
		using IpcConnection = ipc::IpcConnection<Tsubscriber_id, Request, Response>;
		Server(IpcConnection *connection, const Tsubscriber_id& id, Vendor *vendor);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;

		virtual ~Server() noexcept = default;

		void run();
		bool is_running() const;
		void stop();
	private:
		IpcConnection *m_connection;
		Tsubscriber_id m_id;
		Vendor *m_vendor;

		void handle_request(const Request& request) const;
	};

	template <typename Tsubscriber_id>
	inline Server<Tsubscriber_id>::Server(IpcConnection *connection, const Tsubscriber_id& id, Vendor *vendor): m_connection(connection), m_id(id), m_vendor(vendor) {
		if (!m_connection || !m_vendor) {
			throw std::invalid_argument("invalid arguments received");
		}
	}

	template <typename Tsubscriber_id>
	inline void Server<Tsubscriber_id>::run() {
		m_connection->subscribe(
			m_id,
			[this](const Request& request) {
				handle_request(request);
			}
		);
	}

	template <typename Tsubscriber_id>
	inline bool Server<Tsubscriber_id>::is_running() const {
		return m_connection->is_subscribed(m_id);
	}

	template <typename Tsubscriber_id>
	inline void Server<Tsubscriber_id>::stop() {
		m_connection->unsubscribe(m_id);
	}

	template <typename Tsubscriber_id>
	inline void Server<Tsubscriber_id>::handle_request(const Request& request) const {
		try {
			auto response(m_vendor->run_request(request));
			m_connection->send(response);
		} catch (const ServerException& e) {
			m_connection->send(Response(e.code(), e.body()));
		} catch (const std::exception& e) {
			Body body;
			body.add("what", String(std::string(e.what())));
			m_connection->send(Response(ResponseCode::UNSPECIFIED, body));
		}
	}
}

#endif // SERVER_HPP