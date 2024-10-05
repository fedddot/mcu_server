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
	class Server {
	public:
		Server(ipc::IpcConnection *connection, const std::string& id, const Resource& resources_vendor);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;

		virtual ~Server() noexcept = default;

		void start();
		bool is_running() const;
		void stop();
	private:
		ipc::IpcConnection *m_connection;
		std::string m_id;
		std::unique_ptr<Resource> m_resources_vendor;

		void handle_request(const Request& request) const;
	};

	inline Server::Server(ipc::IpcConnection *connection, const std::string& id, const Resource& resources_vendor): m_connection(connection), m_id(id), m_resources_vendor(resources_vendor.clone()) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	inline void Server::start() {
		m_connection->subscribe(
			m_id,
			[this](const Request& request) {
				handle_request(request);
			}
		);
	}

	inline bool Server::is_running() const {
		return m_connection->is_subscribed(m_id);
	}

	inline void Server::stop() {
		m_connection->unsubscribe(m_id);
	}

	inline void Server::handle_request(const Request& request) const {
		try {
			auto response(m_resources_vendor->run_request(request));
			m_connection->send(response);
		} catch (const std::exception& e) {
			Response::Body body;
			body.add("what", String(std::string(e.what())));
			m_connection->send(Response(Response::ResponseCode::UNSPECIFIED, body));
		}
	}
}

#endif // SERVER_HPP