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
		Server(ipc::IpcConnection *connection, const Resource& resources_vendor);
		Server(const Server& other) = delete;
		Server& operator=(const Server& other) = delete;

		virtual ~Server() noexcept;

		void start();
		bool is_running() const;
		void stop();
	private:
		ipc::IpcConnection *m_connection;
		std::unique_ptr<Resource> m_resources_vendor;
	};

	inline Server::Server(ipc::IpcConnection *connection, const Resource& resources_vendor): m_connection(connection), m_resources_vendor(resources_vendor.clone()) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
		m_connection->set_callback(
			[this](const Request& request) {
				try {
					auto response(m_resources_vendor->run_request(request));
					m_connection->send(response);
				} catch (const std::exception& e) {
					Response::Body body;
					body.add("what", String(std::string(e.what())));
					m_connection->send(Response(Response::ResponseCode::UNSPECIFIED, body));
				}
			}
		);
	}

	inline Server::~Server() noexcept {
		m_connection->set_callback([](const Request& request) { });
	}
}

#endif // SERVER_HPP