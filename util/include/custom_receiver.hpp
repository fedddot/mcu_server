#ifndef	CUSTOM_RECEIVER_HPP
#define	CUSTOM_RECEIVER_HPP

#include <memory>
#include <stdexcept>
#include <string>

#include "data_receiver.hpp"
#include "listener.hpp"

namespace mcu_server_utl {
	using RawData = std::string;
	class CustomReceiver: public mcu_server::DataReceiver<RawData, RawData> {
	public:
		CustomReceiver(const RawData& header, const RawData& tail): m_header(header), m_tail(tail) {

		}
		CustomReceiver(const CustomReceiver& other) = delete;
		CustomReceiver& operator=(const CustomReceiver& other) = delete;

		void set_listener(const mcu_server::Listener<RawData>& listener) override {
			m_listener = std::unique_ptr<ReceiverListener>(listener.clone());
		}
		void unset_listener() override {
			m_listener = nullptr;
		}
		void feed(const RawData& food) override {
			m_data.insert(m_data.end(), food.begin(), food.end());
			while (is_extractable() && m_listener) {
				auto extracted_data = extract();
				m_listener->on_event(extracted_data);
			}
		}
	private:
		RawData m_header;
		RawData m_tail;
		RawData m_data;

		using ReceiverListener = mcu_server::Listener<RawData>;
		std::unique_ptr<ReceiverListener> m_listener;

		bool is_extractable() const {
			auto header_pos = m_data.find(m_header);
			auto tail_pos = m_data.find(m_tail, header_pos);
			return ((RawData::npos != header_pos) && (RawData::npos != tail_pos));
		}
		RawData extract() {
			auto header_pos = m_data.find(m_header);
			if (RawData::npos == header_pos) {
				throw std::invalid_argument("missing header");
			}
			auto tail_pos = m_data.find(m_tail, header_pos);
			if (RawData::npos == tail_pos) {
				throw std::invalid_argument("missing tail");
			}
			RawData extracted_data(
				m_data.begin() + header_pos + m_header.size(),
				m_data.begin() + tail_pos
			);
			m_data.erase(
				m_data.begin() + header_pos,
				m_data.begin() + tail_pos + m_tail.size()
			);
			return extracted_data;
		}
	};
}

#endif // CUSTOM_RECEIVER_HPP