#ifndef	PROTOBUF_IPC_SERVER_HPP
#define	PROTOBUF_IPC_SERVER_HPP

#include <functional>
#include <stdexcept>

#include "ipc_server.hpp"

namespace ipc {
	/// @brief Customizable synchronous IPC server aggregating an external raw data buffer
	template <typename Request, typename Response, typename RawDataBuffer>
	class ProtobufIpcServer: public IpcServer<Request, Response> {
	public:
		using RequestCallback = typename IpcServer<Request, Response>::RequestCallback;

		/// @brief Tests receiving data buffer for incoming requests
		/// @return true if data buffer contains a request data and it can be read from the buffer
		using RequestCapturer = std::function<bool(const RawDataBuffer&)>;
		
		/// @brief Extracts a request from raw data buffer erasing its raw data
		/// @return extracted request
		using RequestReader = std::function<Request(RawDataBuffer *)>;

		/// @brief Writes response data into output IPC stream
		using ResponseWriter = std::function<void(const Response&)>;
		
		ProtobufIpcServer(
			RawDataBuffer *raw_data,
			const RequestCapturer& request_capturer,
			const RequestReader& request_reader,
			const ResponseWriter& response_writer
		);
		ProtobufIpcServer(const ProtobufIpcServer&) = delete;
		ProtobufIpcServer& operator=(const ProtobufIpcServer&) = delete;
		
		void serve_once(const RequestCallback& request_callback) const override;
	private:
		RawDataBuffer *m_raw_data;
		RequestCapturer m_request_capturer;
		RequestReader m_request_reader;
		ResponseWriter m_response_writer;
	};

	template <typename Request, typename Response, typename RawDataBuffer>
	inline ProtobufIpcServer<Request, Response, RawDataBuffer>::ProtobufIpcServer(
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

	template <typename Request, typename Response, typename RawDataBuffer>
	inline void ProtobufIpcServer<Request, Response, RawDataBuffer>::serve_once(const RequestCallback& request_callback) const {
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