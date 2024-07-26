#ifndef	BUFFERED_MESSAGE_RECEIVER_HPP
#define	BUFFERED_MESSAGE_RECEIVER_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

#include "message_receiver.hpp"

namespace mcu_platform_utl {

	class BufferedReceiver: public mcu_platform::MessageReceiver<std::string> {
	public:
		BufferedReceiver(const std::string& header, const std::string& tail, const std::size_t& max_buffer_size);
		BufferedReceiver(const BufferedReceiver& other) = delete;
		BufferedReceiver& operator=(const BufferedReceiver& other) = delete;

		bool message_received() const override;
		std::string receive() override;
		void feed(const std::string& food);
	private:
		std::string m_header;
		std::string m_tail;
		std::size_t m_max_buffer_size;
		std::string m_data;
	};

	inline BufferedReceiver::BufferedReceiver(const std::string& header, const std::string& tail, const std::size_t& max_buffer_size): m_header(header), m_tail(tail), m_max_buffer_size(max_buffer_size) {

	}

	inline bool BufferedReceiver::message_received() const {
		auto header_pos = m_data.find(m_header);
		if (std::string::npos == header_pos) {
			return false;
		}
		auto tail_pos = m_data.find(m_tail, header_pos + m_header.size());
		return (std::string::npos != tail_pos);
	}

	inline std::string BufferedReceiver::receive() {
		auto header_pos = m_data.find(m_header);
		if (std::string::npos == header_pos) {
			throw std::invalid_argument("missing header");
		}
		auto tail_pos = m_data.find(m_tail, header_pos + m_header.size());
		if (std::string::npos == tail_pos) {
			throw std::invalid_argument("missing tail");
		}
		std::string extracted_data(
			m_data.begin() + header_pos + m_header.size(),
			m_data.begin() + tail_pos
		);
		m_data.erase(
			m_data.begin() + header_pos,
			m_data.begin() + tail_pos + m_tail.size()
		);
		return extracted_data;
	}

	inline void BufferedReceiver::feed(const std::string& food) {
		if (m_max_buffer_size < m_data.size() + food.size()) {
			throw std::runtime_error("receiving buffer overflow");
		}
		m_data.insert(m_data.end(), food.begin(), food.end());
	}
}

#endif // BUFFERED_MESSAGE_RECEIVER_HPP