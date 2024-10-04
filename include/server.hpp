#ifndef	SERVER_HPP
#define	SERVER_HPP

#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#include "ipc_connection.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace server {
	class Server {
	public:
		Server(ipc::IpcConnection *connection, const Resource& resources_vendor);
		Server(const Server& other);
		Server& operator=(const Server& other) = delete;

		virtual ~Server() noexcept = default;

		void start();
		bool is_running() const;
		void stop();
	private:
		ipc::IpcConnection *m_connection;
		std::unique_ptr<Resource> m_resources_vendor;
		bool m_is_running;

		void server_iteration();
	};

	inline Server::Server(ipc::IpcConnection *connection, const Resource& resources_vendor): m_connection(connection), m_resources_vendor(resources_vendor.clone()), m_is_running(false) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	inline Server::Server(const Server& other): m_connection(other.m_connection), m_resources_vendor(other.m_resources_vendor->clone()), m_is_running(false) {

	}

	inline void Server::start() {
		if (is_running()) {
			return;
		}
		m_is_running = true;
		while (m_is_running) {
			server_iteration();
		}
	}

	inline bool Server::is_running() const {
		return m_is_running;
	}

	inline void Server::stop() {
		m_is_running = false;
	}

	inline void Server::server_iteration() {
		try {
			if (!(m_connection->readable())) {
				return;
			}
			auto request(m_connection->read());
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