#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#include "ipc_connection.hpp"
#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace server {
	template <typename Tsubscriber_id>
	class Server {
	public:
		using IpcConnection = ipc::IpcConnection<Tsubscriber_id, Request, Response>;
		Server(IpcConnection *connection, const Tsubscriber_id& id, const Resource& resources_vendor);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;

		virtual ~Server() noexcept = default;

		void run();
		bool is_running() const;
		void stop();
	private:
		IpcConnection *m_connection;
		Tsubscriber_id m_id;
		std::unique_ptr<Resource> m_resources_vendor;

		void handle_request(const Request& request) const;
	};

	template <typename Tsubscriber_id>
	inline Server<Tsubscriber_id>::Server(IpcConnection *connection, const Tsubscriber_id& id, const Resource& resources_vendor): m_connection(connection), m_id(id), m_resources_vendor(resources_vendor.clone()) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
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
			auto response(m_resources_vendor->run_request(request));
			m_connection->send(response);
		} catch (const std::exception& e) {
			Response::Body body;
			body.add("what", String(Tsubscriber_id(e.what())));
			m_connection->send(Response(Response::ResponseCode::UNSPECIFIED, body));
		}
	}
}

#endif // SERVER_HPP