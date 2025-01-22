#ifndef	BUFFERED_IPC_SERVER_HPP
#define	BUFFERED_IPC_SERVER_HPP

#include "ipc_option.hpp"
#include "ipc_server.hpp"
#include "buffered_ipc_server_config.hpp"

namespace ipc {
	template <typename Request, typename Response, typename RawData>
	class BufferedIpcServer: public IpcServer<Request, Response> {
	public:
		BufferedIpcServer(const BufferedIpcServerConfig<Request, Response, RawData>& config);
		BufferedIpcServer(const BufferedIpcServer&) = delete;
		BufferedIpcServer& operator=(const BufferedIpcServer&) = delete;

		void write(const Response& response) const override;
		Option<Request> read() override;

		void feed(const char ch);
	private:
		BufferedIpcServerConfig<Request, Response, RawData> m_config;
		RawData m_buffer;
	};

	template <typename Request, typename Response, typename RawData>
	inline BufferedIpcServer<Request, Response, RawData>::BufferedIpcServer(const BufferedIpcServerConfig<Request, Response, RawData>& config): m_config(config) {
		m_buffer.reserve(m_config.realloc_size());
	}

	template <typename Request, typename Response, typename RawData>
	inline void BufferedIpcServer<Request, Response, RawData>::write(const Response& outgoing_data) const {
		m_config.writer()(outgoing_data);
	}

	template <typename Request, typename Response, typename RawData>
	Option<Request> BufferedIpcServer<Request, Response, RawData>::read() {
		return m_config.reader()(&m_buffer);
	}

	template <typename Request, typename Response, typename RawData>
	void BufferedIpcServer<Request, Response, RawData>::feed(const char ch) {
		if (m_buffer.size() == m_buffer.capacity()) {
			m_buffer.reserve(m_buffer.size() + m_config.realloc_size());
		}
		m_buffer.push_back(ch);
	}
}

#endif // BUFFERED_IPC_SERVER_HPP