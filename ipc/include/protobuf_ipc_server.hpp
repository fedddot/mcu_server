#ifndef	BUFFERED_IPC_SERVER_HPP
#define	BUFFERED_IPC_SERVER_HPP

#include <functional>
#include <stdexcept>
#include <vector>

#include "pb.h"

#include "ipc_option.hpp"
#include "ipc_server.hpp"

namespace ipc {
	template <typename Request, typename Response>
	class ProtobufIpcServer: public IpcServer<Request, Response> {
	public:
		using RequestStreamReader = std::function<Option<Request>(pb_istream_t *)>;
		using ResponseStreamWriter = std::function<void(pb_ostream_t *, const Response&)>;
		using Handler = typename IpcServer<Request, Response>::Handler;

		ProtobufIpcServer(
			const RequestStreamReader& request_stream_reader,
			const ResponseStreamWriter& response_stream_writer
		);
		ProtobufIpcServer(const ProtobufIpcServer&) = delete;
		ProtobufIpcServer& operator=(const ProtobufIpcServer&) = delete;

		void serve(const Handler& handler) override;
		void stop() override;

		void feed(const pb_byte_t byte);
	private:
		const RequestStreamReader m_request_stream_reader;
		const ResponseStreamWriter m_response_stream_writer;
		
		bool m_is_running;
		std::vector<pb_byte_t> m_buffer;
		std::vector<Request> m_requests_queue;

		enum: int {RESIZE_FACTOR = 10UL};

		void serve_once(const Handler& handler);
	};

	template <typename Request, typename Response>
	inline ProtobufIpcServer<Request, Response>::ProtobufIpcServer(
		const RequestStreamReader& request_stream_reader,
		const ResponseStreamWriter& response_stream_writer
	): m_request_stream_reader(request_stream_reader), m_response_stream_writer(response_stream_writer), m_is_running(false) {
		m_buffer.reserve(RESIZE_FACTOR);
	}

	template <typename Request, typename Response>
	void ProtobufIpcServer<Request, Response>::serve(const Handler& handler) {
		if (!handler) {
			throw std::invalid_argument("invalid request handler received");
		}
		m_is_running = true;
		while (m_is_running) {
			serve_once(handler);
		}
	}

	template <typename Request, typename Response>
	void ProtobufIpcServer<Request, Response>::serve_once(const Handler& handler) {
		if (m_requests_queue.empty()) {
			return;
		}
		const auto request_iter = m_requests_queue.begin();
		const auto response = handler(*request_iter);
		m_response_stream_writer(&m_buffer, response);
		m_requests_queue.erase(request_iter);
	}
	
	template <typename Request, typename Response>
	void ProtobufIpcServer<Request, Response>::stop() {
		m_is_running = false;
	}

	template <typename Request, typename Response>
	void ProtobufIpcServer<Request, Response>::feed(const pb_byte_t ch) {
		if (m_buffer.size() == m_buffer.capacity()) {
			m_buffer.reserve(m_buffer.size() + RESIZE_FACTOR);
		}
		m_buffer.push_back(ch);
		const auto proto_request_opt = m_request_stream_reader(&m_buffer);
		if (!proto_request_opt.some()) {
			return;
		}
		const auto request = m_request_stream_reader(proto_request_opt.get());
		m_requests_queue.push_back(request);
	}
}

#endif // BUFFERED_IPC_SERVER_HPP