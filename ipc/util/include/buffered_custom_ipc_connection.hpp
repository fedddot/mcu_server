#ifndef	BUFFERED_CUSTOM_IPC_CONNECTION_HPP
#define	BUFFERED_CUSTOM_IPC_CONNECTION_HPP

#include <functional>
#include <stdexcept>

#include "ipc_connection.hpp"
#include "request.hpp"
#include "response.hpp"

namespace server_utl {
	template <typename Traw_data>
	class BufferedCustomIpcConnection: public server::IpcConnection {
	public:
		using RequestParser = std::function<server::Request(const Traw_data&)>;
		using ResponseSerializer = std::function<Traw_data(const server::Response&)>;
		using RawDataSender = std::function<void(const Traw_data&)>;
		
		BufferedCustomIpcConnection(
			const Traw_data& head,
			const Traw_data& tail,
			const RequestParser& request_parser,
			const ResponseSerializer& response_serializer,
			const RawDataSender& raw_data_sender
		);
		BufferedCustomIpcConnection(const BufferedCustomIpcConnection& other) = default;
		BufferedCustomIpcConnection& operator=(const BufferedCustomIpcConnection& other) = default;

		bool readable() const override;
		server::Request read() override;
		void send(const server::Response& response) const override;
		void feed(const Traw_data& data);
	private:
		Traw_data m_head;
		Traw_data m_tail;
		RequestParser m_request_parser;
		ResponseSerializer m_response_serializer;
		RawDataSender m_raw_data_sender;

		Traw_data m_data;
	};

	template <typename Traw_data>
	inline BufferedCustomIpcConnection<Traw_data>::BufferedCustomIpcConnection(
			const Traw_data& head,
			const Traw_data& tail,
			const RequestParser& request_parser,
			const ResponseSerializer& response_serializer,
			const RawDataSender& raw_data_sender
		): m_head(head), m_tail(tail), m_request_parser(request_parser), m_response_serializer(response_serializer), m_raw_data_sender(raw_data_sender) {
		
		if (m_head.empty() || m_tail.empty()) {
			throw std::invalid_argument("invalid head/tail received");
		}
		if (!m_request_parser || !m_response_serializer || !m_raw_data_sender) {
			throw std::invalid_argument("invalid action received");
		}
	}

	template <typename Traw_data>
	inline bool BufferedCustomIpcConnection<Traw_data>::readable() const {
		auto header_pos = m_data.find(m_head);
		auto tail_pos = m_data.find(m_tail, header_pos);
		return ((Traw_data::npos != header_pos) && (Traw_data::npos != tail_pos));
	}
	
	template <typename Traw_data>
	inline server::Request BufferedCustomIpcConnection<Traw_data>::read() {
		auto header_pos = m_data.find(m_head);
		if (Traw_data::npos == header_pos) {
			throw std::invalid_argument("missing header");
		}
		auto tail_pos = m_data.find(m_tail, header_pos);
		if (Traw_data::npos == tail_pos) {
			throw std::invalid_argument("missing tail");
		}
		Traw_data extracted_data(
			m_data.begin() + header_pos + m_head.size(),
			m_data.begin() + tail_pos
		);
		m_data.erase(
			m_data.begin() + header_pos,
			m_data.begin() + tail_pos + m_tail.size()
		);
		return m_request_parser(extracted_data);
	}

	template <typename Traw_data>
	inline void BufferedCustomIpcConnection<Traw_data>::send(const server::Response& response) const {
		m_raw_data_sender(m_head + m_response_serializer(response) + m_tail);
	}

	template <typename Traw_data>
	inline void BufferedCustomIpcConnection<Traw_data>::feed(const Traw_data& data) {
		m_data.insert(m_data.end(), data.begin(), data.end());
	}
}

#endif // BUFFERED_CUSTOM_IPC_CONNECTION_HPP