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

	template <typename Trequest, typename Tresponse>
	class BufferedIpcServerConfig: public IpcConfig {
	public:
		using RequestReader = std::function<Option<Trequest>(RawData *)>;
		using ResponseWriter = std::function<void(const Tresponse&)>;
		
		BufferedIpcServerConfig() = default;
		BufferedIpcServerConfig(const BufferedIpcServerConfig&) = default;
		BufferedIpcServerConfig& operator=(const BufferedIpcServerConfig&) = default;

		std::string type() const override;
		
		RequestReader request_reader;
		ResponseWriter response_writer;
		std::size_t realloc_size;
	};

	template <typename Trequest, typename Tresponse>
	inline std::string BufferedIpcServerConfig<Trequest, Tresponse>::type() const {
		return std::string("ipc.server.buffered");
	}

	template <typename Trequest, typename Tresponse>
	class BufferedIpcServer: public IpcServer<Trequest, Tresponse> {
	public:
		BufferedIpcServer(const BufferedIpcServerConfig<Trequest, Tresponse>& config);
		BufferedIpcServer(const BufferedIpcServer&) = delete;
		BufferedIpcServer& operator=(const BufferedIpcServer&) = delete;

		void write(const Tresponse& response) const override;
		Option<Trequest> read() override;

		void feed(const char ch);
	private:
		BufferedIpcServerConfig<Trequest, Tresponse> m_config;
		RawData m_buffer;
	};

	template <typename Trequest, typename Tresponse>
	inline BufferedIpcServer<Trequest, Tresponse>::BufferedIpcServer(const BufferedIpcServerConfig<Trequest, Tresponse>& config): m_config(config) {
		if (!(m_config.request_reader) || !(m_config.response_writer)) {
			throw std::invalid_argument("invalid config received - required fields (reader and/or writer) are uninitialized");
		}
		m_buffer.reserve(m_config.realloc_size);
	}

	template <typename Trequest, typename Tresponse>
	inline void BufferedIpcServer<Trequest, Tresponse>::write(const Tresponse& outgoing_data) const {
		m_config.response_writer(outgoing_data);
	}

	template <typename Trequest, typename Tresponse>
	Option<Trequest> BufferedIpcServer<Trequest, Tresponse>::read() {
		return m_config.request_reader(&m_buffer);
	}

	template <typename Trequest, typename Tresponse>
	void BufferedIpcServer<Trequest, Tresponse>::feed(const char ch) {
		if (m_buffer.size() == m_buffer.capacity()) {
			m_buffer.reserve(m_buffer.size() + m_config.realloc_size);
		}
		m_buffer.push_back(ch);
	}
}

#endif // BUFFERED_IPC_SERVER_HPP