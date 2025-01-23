#ifndef	BUFFERED_IPC_SERVER_HPP
#define	BUFFERED_IPC_SERVER_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <vector>

#include "ipc_option.hpp"
#include "ipc_server.hpp"

namespace ipc {
	template <typename Request, typename Response>
	class BufferedIpcServer: public IpcServer<Request, Response> {
	public:
		using RequestReader = std::function<Option<Request>(std::vector<char> *)>;
		using ResponseWriter = std::function<void(const Response&)>;
		BufferedIpcServer(
			const RequestReader& request_reader,
			const ResponseWriter& response_writer,
			const std::size_t& resize_factor
		);
		BufferedIpcServer(const BufferedIpcServer&) = delete;
		BufferedIpcServer& operator=(const BufferedIpcServer&) = delete;

		void serve(IpcRequestHandler<Request, Response> *request_handler) override;
		void stop() override;

		void feed(const char ch);
	private:
		const RequestReader m_request_reader;
		const ResponseWriter m_response_writer;
		const std::size_t m_resize_factor;
		bool m_is_running;
		std::vector<char> m_buffer;
	};

	template <typename Request, typename Response>
	inline BufferedIpcServer<Request, Response>::BufferedIpcServer(
		const RequestReader& request_reader,
		const ResponseWriter& response_writer,
		const std::size_t& resize_factor
	): m_request_reader(request_reader), m_response_writer(response_writer), m_resize_factor(resize_factor), m_is_running(false) {
		m_buffer.reserve(m_resize_factor);
	}

	template <typename Request, typename Response>
	void BufferedIpcServer<Request, Response>::serve(IpcRequestHandler<Request, Response> *request_handler) {
		if (!request_handler) {
			throw std::invalid_argument("invalid request handler received");
		}
		m_is_running = true;
		while (m_is_running) {
			const auto request = m_request_reader(&m_buffer);
			if (!request.some()) {
				continue;
			}
			const auto response = request_handler->handle(request.get());
			m_response_writer(response);
		}
	}
	
	template <typename Request, typename Response>
	void BufferedIpcServer<Request, Response>::stop() {
		m_is_running = false;
	}

	template <typename Request, typename Response>
	void BufferedIpcServer<Request, Response>::feed(const char ch) {
		if (m_buffer.size() == m_buffer.capacity()) {
			m_buffer.reserve(m_buffer.size() + m_resize_factor);
		}
		m_buffer.push_back(ch);
	}
}

#endif // BUFFERED_IPC_SERVER_HPP