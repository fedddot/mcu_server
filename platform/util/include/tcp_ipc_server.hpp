#ifndef	TCP_IPC_SERVER_HPP
#define	TCP_IPC_SERVER_HPP

#include <stdexcept>
#include <string>

#include "ipc_server.hpp"

namespace mcu_platform_utl {	

	class TcpIpcServer: public mcu_platform::IpcServer<std::string> {
	public:
		TcpIpcServer(const std::string& host, int port, const std::string& header, const std::string& tail);
		~TcpIpcServer() noexcept override;
		bool poll(int timeout_ms) const override;
		std::string receive() override;
		void send(const std::string& data) const override;
	private:
		std::string m_host;
		int m_port;
		std::string m_header;
		std::string m_tail;

		int m_fd;
	};

	inline TcpIpcServer::TcpIpcServer(const std::string& host, int port, const std::string& header, const std::string& tail): m_host(host), m_port(port), m_header(header), m_tail(tail) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	inline TcpIpcServer::~TcpIpcServer() noexcept {
		
	}
	inline bool TcpIpcServer::poll(int timeout_ms) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
	inline std::string TcpIpcServer::receive() {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
	inline void TcpIpcServer::send(const std::string& data) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // TCP_IPC_SERVER_HPP