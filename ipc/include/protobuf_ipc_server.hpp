#ifndef	PROTOBUF_IPC_SERVER_HPP
#define	PROTOBUF_IPC_SERVER_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <vector>

#include "pb.h"

#include "ipc_option.hpp"
#include "ipc_server.hpp"
#include "protobuf_input_stream_builder.hpp"

namespace ipc {
	template <typename Request, typename Response, std::size_t N>
	class ProtobufIpcServer: public IpcServer<Request, Response> {
	public:
		using RequestStreamReader = std::function<Option<Request>(pb_istream_t *)>;
		using ResponseStreamWriter = std::function<void(const Response&)>;
		using Handler = typename IpcServer<Request, Response>::Handler;

		ProtobufIpcServer(
			const RequestStreamReader& request_stream_reader,
			const ResponseStreamWriter& response_stream_writer
		);
		ProtobufIpcServer(const ProtobufIpcServer&) = delete;
		ProtobufIpcServer& operator=(const ProtobufIpcServer&) = delete;

		void serve(const Handler& handler) override;
		void stop() override;

		void serve_once(const Handler& handler);
		void feed(const pb_byte_t byte);
	private:
		const RequestStreamReader m_request_stream_reader;
		const ResponseStreamWriter m_response_stream_writer;
		
		bool m_is_running;
		std::vector<pb_byte_t> m_buffer;
		std::vector<Request> m_requests_queue;
	};

	template <typename Request, typename Response, std::size_t N>
	inline ProtobufIpcServer<Request, Response, N>::ProtobufIpcServer(
		const RequestStreamReader& request_stream_reader,
		const ResponseStreamWriter& response_stream_writer
	): m_request_stream_reader(request_stream_reader), m_response_stream_writer(response_stream_writer), m_is_running(false) {
	}

	template <typename Request, typename Response, std::size_t N>
	inline void ProtobufIpcServer<Request, Response, N>::serve(const Handler& handler) {
		if (!handler) {
			throw std::invalid_argument("invalid request handler received");
		}
		m_is_running = true;
		while (m_is_running) {
			serve_once(handler);
		}
	}

	template <typename Request, typename Response, std::size_t N>
	inline void ProtobufIpcServer<Request, Response, N>::serve_once(const Handler& handler) {
		if (m_requests_queue.empty()) {
			return;
		}
		const auto request_iter = m_requests_queue.begin();
		const auto response = handler(*request_iter);
		m_response_stream_writer(response);
		m_requests_queue.erase(request_iter);
	}
	
	template <typename Request, typename Response, std::size_t N>
	inline void ProtobufIpcServer<Request, Response, N>::stop() {
		m_is_running = false;
	}

	template <typename Request, typename Response, std::size_t N>
	inline void ProtobufIpcServer<Request, Response, N>::feed(const pb_byte_t ch) {
		m_buffer.push_back(ch);
		ProtobufInputStreamBuilder read_stream_builder(&m_buffer);
		auto read_stream = read_stream_builder.build();
		const auto bytes_to_read_before = read_stream.bytes_left;
		const auto proto_request_opt = m_request_stream_reader(&read_stream);
		if (!proto_request_opt.some()) {
			return;
		}
		const auto bytes_to_read_after = read_stream.bytes_left;
		if (bytes_to_read_after > bytes_to_read_before) {
			throw std::runtime_error("read buffer somehow got bigger after reading operation");
		}
		auto bytes_consumed = bytes_to_read_before - bytes_to_read_after;
		m_buffer.erase(m_buffer.begin(), m_buffer.begin() + bytes_consumed);
		m_requests_queue.push_back(proto_request_opt.get());
	}
}

#endif // PROTOBUF_IPC_SERVER_HPP