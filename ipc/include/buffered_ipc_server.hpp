#ifndef	BUFFERED_IPC_SERVER_HPP
#define	BUFFERED_IPC_SERVER_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <vector>

#include "ipc_config.hpp"
#include "ipc_option.hpp"
#include "ipc_server.hpp"

namespace ipc {
	using RawData = std::vector<char>;

	template <typename Request, typename Response>
	class BufferedIpcServerConfig: public IpcConfig {
	public:
		using RequestReader = std::function<Option<Request>(RawData *)>;
		using ResponseWriter = std::function<void(const Response&)>;
		
		BufferedIpcServerConfig() = default;
		BufferedIpcServerConfig(const BufferedIpcServerConfig&) = default;
		BufferedIpcServerConfig& operator=(const BufferedIpcServerConfig&) = default;

		std::string type() const override;
		
		RequestReader request_reader;
		ResponseWriter response_writer;
		std::size_t realloc_size;
	};

	template <typename Request, typename Response>
	inline std::string BufferedIpcServerConfig<Request, Response>::type() const {
		return std::string("ipc.server.buffered");
	}

	template <typename Request, typename Response>
	class BufferedIpcServer: public IpcServer<Request, Response> {
	public:
		BufferedIpcServer(const BufferedIpcServerConfig<Request, Response>& config);
		BufferedIpcServer(const BufferedIpcServer&) = delete;
		BufferedIpcServer& operator=(const BufferedIpcServer&) = delete;

		void write(const Response& response) const override;
		Option<Request> read() override;

		void feed(const char ch);
	private:
		BufferedIpcServerConfig<Request, Response> m_config;
		RawData m_buffer;
	};

	template <typename Request, typename Response>
	inline BufferedIpcServer<Request, Response>::BufferedIpcServer(const BufferedIpcServerConfig<Request, Response>& config): m_config(config) {
		if (!(m_config.request_reader) || !(m_config.response_writer)) {
			throw std::invalid_argument("invalid config received - required fields (reader and/or writer) are uninitialized");
		}
		m_buffer.reserve(m_config.realloc_size);
	}

	template <typename Request, typename Response>
	inline void BufferedIpcServer<Request, Response>::write(const Response& outgoing_data) const {
		m_config.response_writer(outgoing_data);
	}

	template <typename Request, typename Response>
	Option<Request> BufferedIpcServer<Request, Response>::read() {
		return m_config.request_reader(&m_buffer);
	}

	template <typename Request, typename Response>
	void BufferedIpcServer<Request, Response>::feed(const char ch) {
		if (m_buffer.size() == m_buffer.capacity()) {
			m_buffer.reserve(m_buffer.size() + m_config.realloc_size);
		}
		m_buffer.push_back(ch);
	}
}

#endif // BUFFERED_IPC_SERVER_HPP