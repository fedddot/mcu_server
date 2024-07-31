#ifndef	TCP_IPC_SERVER_HPP
#define	TCP_IPC_SERVER_HPP

#include <asm-generic/socket.h>
#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstring>
#include <memory>
#include <mutex>
#include <netdb.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <poll.h>

#include "buffered_message_receiver.hpp"
#include "ipc_server.hpp"

namespace mcu_platform_utl {	

	class TcpIpcServer: public mcu_platform::IpcServer<std::string> {
	public:
		TcpIpcServer(const std::string& host, int port, const std::string& header, const std::string& tail, const std::size_t& max_buffer_size);
		~TcpIpcServer() noexcept override;
		bool poll(int timeout_ms) const override;
		std::string receive() override;
		void send(const std::string& data) const override;
	private:
		std::string m_header;
		std::string m_tail;

		class SocketData {
		public:
			SocketData(int fd, const sockaddr& sockaddr, const socklen_t& socklen);
			SocketData(const SocketData& other) = default;
			SocketData& operator=(const SocketData& other) = default;
			~SocketData() noexcept = default;

			int fd() const;
		private:
			int m_fd;
			sockaddr m_sockaddr;
			socklen_t m_socklen;
		};

		SocketData m_socket_data;
		int m_client_fd;
		mutable std::mutex m_client_mux;
		mutable std::condition_variable m_client_cond;

		BufferedReceiver m_receiver;
		mutable std::mutex m_receiver_mux;
		mutable std::condition_variable m_receiver_cond;

		std::thread m_listening_thread;
		bool m_is_running;
		static SocketData init_socket(const std::string& host, int port);

		static std::string read_data(int fd);
		static short poll_fd(int fd, short events, int timeout_ms);
		static int wait_connection(int fd, int timeout_ms);
	};

	inline TcpIpcServer::TcpIpcServer(const std::string& host, int port, const std::string& header, const std::string& tail, const std::size_t& max_buffer_size): m_header(header), m_tail(tail), m_socket_data(init_socket(host, port)), m_client_fd(-1), m_receiver(header, tail, max_buffer_size), m_is_running(false) {
		m_is_running = true;
		m_listening_thread = std::thread(
			[this](void) {
				enum {POLLING_TIMOUT_MS = 100};
				while (m_is_running) {
					std::unique_lock lock(m_client_mux);
					if (0 > m_client_fd) {
						m_client_fd = wait_connection(m_socket_data.fd(), POLLING_TIMOUT_MS);
						if (0 > m_client_fd) {
							continue;
						}
						m_client_cond.notify_one();
					}
					auto revents = poll_fd(m_client_fd, POLLIN | POLLERR | POLLHUP, POLLING_TIMOUT_MS);
					if (revents & (POLLERR | POLLHUP)) {
						close(m_client_fd);
						m_client_fd = -1;
						continue;
					}

					std::unique_lock receiver_lock(m_receiver_mux);
					if (revents & POLLIN) {
						m_receiver.feed(read_data(m_client_fd));
					}

					if (m_receiver.message_received()) {
						m_receiver_cond.notify_one();
					}
				}
			}
		);
	}

	inline TcpIpcServer::~TcpIpcServer() noexcept {
		close(m_socket_data.fd());
	}
	
	inline bool TcpIpcServer::poll(int timeout_ms) const {
		std::unique_lock lock(m_receiver_mux);
		if (m_receiver.message_received()) {
			return true;
		}
		return std::cv_status::no_timeout == m_receiver_cond.wait_for(lock, std::chrono::milliseconds(timeout_ms));
	}

	inline std::string TcpIpcServer::receive() {
		std::unique_lock lock(m_receiver_mux);
		if (!m_receiver.message_received()) {
			throw std::runtime_error("receiving buffer is empty");
		}
		return m_receiver.receive();
	}

	inline void TcpIpcServer::send(const std::string& data) const {
		std::unique_lock lock(m_client_mux);
		if (0 > m_client_fd) {
			throw std::runtime_error("client is not connected");
		}
		
	}

	inline TcpIpcServer::SocketData TcpIpcServer::init_socket(const std::string& host, int port) {
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (0 > fd) {
			throw std::runtime_error("failed to create socket, errno = " + std::to_string(errno));
		} 
		int opt = 1;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
			close(fd);
			throw std::runtime_error("failed to set socket options");
		}

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
		sockaddr sockaddr;
		socklen_t socklen;
		while (list_iter) {
			auto bind_res = bind(fd, list_iter->ai_addr, list_iter->ai_addrlen);
			if (0 == bind_res) {
				std::memcpy(&sockaddr, list_iter->ai_addr, static_cast<std::size_t>(list_iter->ai_addrlen));
				socklen = list_iter->ai_addrlen;
				break;
			}
			list_iter = list_iter->ai_next;
		}
		freeaddrinfo(list);
		list = nullptr;
		if (nullptr == list_iter) {
			close(fd);
			throw std::runtime_error("failed to bind socket " + std::to_string(fd) + " to addr " + host + ":" + std::to_string(port));
		}
		list_iter = nullptr;
		if (listen(fd, 1) < 0) {
			close(fd);
			throw std::runtime_error("failed to start listening");
		}
		return SocketData(fd, sockaddr, socklen);
	}
}

#endif // TCP_IPC_SERVER_HPP