#ifndef	TCP_IPC_SERVER_HPP
#define	TCP_IPC_SERVER_HPP

#include <asm-generic/socket.h>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <netdb.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

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
		std::string m_header;
		std::string m_tail;

		int m_fd;
		sockaddr m_addr;
		socklen_t m_addr_len;

		void bind_socket(int fd, const std::string& host, int port);
	};

	inline void TcpIpcServer::bind_socket(int fd, const std::string& host, int port) {
	    addrinfo hints;
    	std::memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;	

		addrinfo *list(nullptr);
    	auto info_res = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &list);
		if (0 != info_res) {
			close(fd);
			throw std::runtime_error("failed to get addrinfo, errno = " + std::to_string(errno));
		}
		auto list_iter = list;
		while (list_iter) {
			auto bind_res = bind(fd, list_iter->ai_addr, list_iter->ai_addrlen);
			if (0 == bind_res) {
				std::memcpy(&m_addr, list_iter->ai_addr, static_cast<std::size_t>(list_iter->ai_addrlen));
				m_addr_len = list_iter->ai_addrlen;
				break;
			}
			list_iter = list_iter->ai_next;
		}
		freeaddrinfo(list);
		list = 0;
		if (nullptr == list_iter) {
			close(fd);
			throw std::runtime_error("failed to bind socket " + std::to_string(fd) + " to addr " + host + ":" + std::to_string(port));
		}
	}

	inline TcpIpcServer::TcpIpcServer(const std::string& host, int port, const std::string& header, const std::string& tail): m_header(header), m_tail(tail) {
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (0 > fd) {
			throw std::runtime_error("failed to create socket, errno = " + std::to_string(errno));
		} 
		int opt = 1;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			close(fd);
			throw std::runtime_error("failed to set socket options");
		}
		bind_socket(fd, host, port);
		if (listen(fd, 1) < 0) {
			close(fd);
			throw std::runtime_error("failed to start listening");
		}
		m_fd = fd;
	}

	inline TcpIpcServer::~TcpIpcServer() noexcept {
		close(m_fd);
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