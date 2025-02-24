#ifndef	PROTOBUF_IPC_SERVER_HPP
#define	PROTOBUF_IPC_SERVER_HPP

#include <functional>
#include <stdexcept>

#include "pb.h"

#include "ipc_server.hpp"
#include "custom_ipc_server.hpp"

namespace ipc {
	/// @brief Customizable synchronous Protobuf IPC server aggregating an external raw data buffer
	template <typename Request, typename Response>
	class ProtobufIpcServer: public IpcServer<Request, Response> {
	public:
		using RequestCallback = typename IpcServer<Request, Response>::RequestCallback;
		using RequestCapturer = typename CustomIpcServer<Request, Response, pb_callback_t>::RequestCapturer;
		using RequestReader = typename CustomIpcServer<Request, Response, pb_callback_t>::RequestReader;
		using ResponseWriter = typename CustomIpcServer<Request, Response, pb_callback_t>::ResponseWriter;

		ProtobufIpcServer(
		);
		ProtobufIpcServer(const ProtobufIpcServer&) = delete;
		ProtobufIpcServer& operator=(const ProtobufIpcServer&) = delete;
		
		void serve_once(const RequestCallback& request_callback) const override;
	private:
		pb_callback_t *m_buff;
		CustomIpcServer<Request, Response, pb_istream_t> m_server;
		
		bool capture_ipc_msg(const pb_istream_t& buff) const;
		Request read_ipc_msg(pb_istream_t *buff);

		/// @brief Writes response data into output IPC stream
		using ResponseWriter = std::function<void(const Response&)>;
	};

	template <typename Request, typename Response>
	inline ProtobufIpcServer<Request, Response>::ProtobufIpcServer(
		RawDataBuffer *raw_data,
		const RequestCapturer& request_capturer,
		const RequestReader& request_reader,
		const ResponseWriter& response_writer
	): m_raw_data(raw_data), m_request_capturer(request_capturer), m_request_reader(request_reader), m_response_writer(response_writer) {
		if (!m_raw_data) {
			throw std::invalid_argument("invalid raw data buffer pointer received");
		}
		if (!m_request_capturer || !m_request_reader || !m_response_writer) {
			throw std::invalid_argument("one or more of injected functions are invalid");
		}
	}

	template <typename Request, typename Response>
	inline void ProtobufIpcServer<Request, Response>::serve_once(const RequestCallback& request_callback) const {
		if (!m_request_capturer(std::ref(*m_raw_data))) {
			return;
		}
		if (!request_callback) {
			throw std::invalid_argument("invalid request callback received");
		}
		const auto request = m_request_reader(m_raw_data);
		const auto response = request_callback(request);
		m_response_writer(response);
	}
}

#endif // PROTOBUF_IPC_SERVER_HPP