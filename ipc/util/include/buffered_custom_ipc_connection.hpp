#ifndef	BUFFERED_CUSTOM_IPC_CONNECTION_HPP
#define	BUFFERED_CUSTOM_IPC_CONNECTION_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>

#include "ipc_connection.hpp"

namespace mcu_ipc_utl {
	template <typename Tdata>
	class BufferedCustomIpcConnection: public mcu_ipc::IpcConnection<Tdata> {
	public:
		using SendRawDataFunction = std::function<void(const Tdata&)>;
		BufferedCustomIpcConnection(const Tdata& head, const Tdata& tail, const std::size_t& max_buff_size, const SendRawDataFunction& send_raw_data);
		BufferedCustomIpcConnection(const BufferedCustomIpcConnection& other) = delete;
		BufferedCustomIpcConnection& operator=(const BufferedCustomIpcConnection& other) = delete;
		
		bool readable() const override;
		Tdata read() override;
		void send(const Tdata& data) const override;
		void feed(const Tdata& data);
	private:
		const Tdata m_head;
		const Tdata m_tail;
		const std::size_t m_max_buff_size;
		const SendRawDataFunction m_send_raw_data;

		Tdata m_data;
	};

	template <typename Tdata>
	inline BufferedCustomIpcConnection<Tdata>::BufferedCustomIpcConnection(const Tdata& head, const Tdata& tail, const std::size_t& max_buff_size, const SendRawDataFunction& send_raw_data): m_head(head), m_tail(tail), m_max_buff_size(max_buff_size), m_send_raw_data(send_raw_data) {
		if (!m_send_raw_data) {
			throw std::invalid_argument("invalid sender received");
		}
	}

	template <typename Tdata>
	inline bool BufferedCustomIpcConnection<Tdata>::readable() const {
		auto header_pos = m_data.find(m_head);
		auto tail_pos = m_data.find(m_tail, header_pos);
		return ((Tdata::npos != header_pos) && (Tdata::npos != tail_pos));
	}
	
	template <typename Tdata>
	inline Tdata BufferedCustomIpcConnection<Tdata>::read() {
		auto header_pos = m_data.find(m_head);
		if (Tdata::npos == header_pos) {
			throw std::invalid_argument("missing header");
		}
		auto tail_pos = m_data.find(m_tail, header_pos);
		if (Tdata::npos == tail_pos) {
			throw std::invalid_argument("missing tail");
		}
		Tdata extracted_data(
			m_data.begin() + header_pos + m_head.size(),
			m_data.begin() + tail_pos
		);
		m_data.erase(
			m_data.begin() + header_pos,
			m_data.begin() + tail_pos + m_tail.size()
		);
		return extracted_data;
	}

	template <typename Tdata>
	inline void BufferedCustomIpcConnection<Tdata>::send(const Tdata& data) const {
		m_send_raw_data(data);
	}

	template <typename Tdata>
	inline void BufferedCustomIpcConnection<Tdata>::feed(const Tdata& data) {
		m_data.insert(m_data.end(), data.begin(), data.end());
	}
}

#endif // BUFFERED_CUSTOM_IPC_CONNECTION_HPP