#ifndef	SERIAL_CONNECTION_HPP
#define	SERIAL_CONNECTION_HPP

#include <functional>
#include <stdexcept>
#include <string>

#include "connection.hpp"

namespace mcu_control_utl {

    using SerialData = std::string;

	class SerialConnection: public mcu_control::Connection<SerialData> {
	public:
        using SendAction = std::function<void(const SerialData&)>;
		SerialConnection(const SendAction& send_action, const SerialData& head, const SerialData& tail);
		SerialConnection(const SerialConnection& other) = delete;
		SerialConnection& operator=(const SerialConnection& other) = delete;

		void send_data(const SerialData& data) const override;
		bool is_readable() const override;
		SerialData read_data() const override;
        void feed_data(const SerialData& data);
	private:
        SendAction m_send_action;
        SerialData m_head;
        SerialData m_tail;
        mutable SerialData m_data_buffer;
        SerialData wrap_data(const SerialData& data) const;
	};

	inline SerialConnection::SerialConnection(const SendAction& send_action, const SerialData& head, const SerialData& tail): m_send_action(send_action), m_head(head), m_tail(tail) {

	}

	inline void SerialConnection::send_data(const SerialData& data) const {
        m_send_action(wrap_data(data));
	}

	inline bool SerialConnection::is_readable() const {
        auto head_pos = m_data_buffer.find(m_head);
        if (std::string::npos == head_pos) {
            return false;
        }
        auto tail_pos = m_data_buffer.find(m_tail, head_pos + m_head.size());
        if (std::string::npos == tail_pos) {
            return false;
        }
		return true;
	}

	inline SerialData SerialConnection::read_data() const {
        auto head_pos = m_data_buffer.find(m_head);
        if (std::string::npos == head_pos) {
            throw std::runtime_error("failed to find data header");
        }
        auto tail_pos = m_data_buffer.find(m_tail, head_pos + m_head.size());
        if (std::string::npos == tail_pos) {
            throw std::runtime_error("failed to find data tail");
        }
        SerialData data(m_data_buffer.begin() + head_pos + m_head.size(), m_data_buffer.begin() + tail_pos);
        m_data_buffer.erase(m_data_buffer.begin() + head_pos, m_data_buffer.begin() + tail_pos + m_tail.size());
		return data;
	}

    inline void SerialConnection::feed_data(const SerialData& data) {
        m_data_buffer.insert(m_data_buffer.end(), data.begin(), data.end());
    }

    inline SerialData SerialConnection::wrap_data(const SerialData& data) const {
        return m_head + data + m_tail;
    }
}
#endif // SERIAL_CONNECTION_HPP