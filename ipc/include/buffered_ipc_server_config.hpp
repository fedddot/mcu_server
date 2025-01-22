#ifndef	BUFFERED_IPC_SERVER_CONFIG_HPP
#define	BUFFERED_IPC_SERVER_CONFIG_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>

#include "ipc_config.hpp"
#include "ipc_option.hpp"

namespace ipc {
	template <typename Request, typename Response, typename RawData>
	class BufferedIpcServerConfig: public IpcConfig {
	public:
		using RequestReader = std::function<Option<Request>(RawData *)>;
		using ResponseWriter = std::function<void(const Response&)>;
		
		BufferedIpcServerConfig(const RequestReader& reader, const ResponseWriter& writer, const std::size_t& realloc_size);
		BufferedIpcServerConfig(const BufferedIpcServerConfig&) = default;
		BufferedIpcServerConfig& operator=(const BufferedIpcServerConfig&) = default;

		std::string type() const override;
		const RequestReader& reader() const;
		const ResponseWriter& writer() const;
		const std::size_t realloc_size() const;
	private:
		RequestReader m_request_reader;
		ResponseWriter m_response_writer;
		std::size_t m_realloc_size;
	};

	template <typename Request, typename Response, typename RawData>
	inline BufferedIpcServerConfig<Request, Response, RawData>::BufferedIpcServerConfig(const RequestReader& reader, const ResponseWriter& writer, const std::size_t& realloc_size): m_request_reader(reader), m_response_writer(writer), m_realloc_size(realloc_size) {
		if (!m_request_reader || !m_response_writer) {
			throw std::invalid_argument("invalid actions received");
		}
	}

	template <typename Request, typename Response, typename RawData>
	inline std::string BufferedIpcServerConfig<Request, Response, RawData>::type() const {
		return std::string("ipc.server.buffered");
	}

	template <typename Request, typename Response, typename RawData>
	inline const typename BufferedIpcServerConfig<Request, Response, RawData>::RequestReader& BufferedIpcServerConfig<Request, Response, RawData>::reader() const {
		return std::ref(m_request_reader);
	}

	template <typename Request, typename Response, typename RawData>
	inline const typename BufferedIpcServerConfig<Request, Response, RawData>::ResponseWriter& BufferedIpcServerConfig<Request, Response, RawData>::writer() const {
		return std::ref(m_response_writer);
	}

	template <typename Request, typename Response, typename RawData>
	inline const std::size_t BufferedIpcServerConfig<Request, Response, RawData>::realloc_size() const {
		return m_realloc_size;
	}
}

#endif // BUFFERED_IPC_SERVER_CONFIG_HPP