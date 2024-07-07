#ifndef	CUSTOM_RECEIVER_HPP
#define	CUSTOM_RECEIVER_HPP

#include <stdexcept>
#include <functional>

#include "data_receiver.hpp"
#include "data_sender.hpp"

namespace mcu_server_utl {
	
	template <typename Tdata>
	class CustomReceiver: public mcu_server::DataReceiver<Tdata> {
	public:
		CustomReceiver();
		CustomReceiver(const CustomReceiver& other) = default;
		CustomReceiver& operator=(const CustomReceiver& other) = default;
				
		void send(const Tdata& data) const override;
		mcu_server::DataSender<Tdata> *clone() const override;
	private:
		SendFunction m_send_function;
	};

	template <typename Tdata>
	inline CustomReceiver<Tdata>::CustomReceiver(const SendFunction& send_function): m_send_function(send_function) {
		if (!m_send_function) {
			throw std::invalid_argument("invalid send action received");
		}
	}

	template <typename Tdata>
	inline void CustomReceiver<Tdata>::send(const Tdata& data) const {
		m_send_function(data);
	}

	template <typename Tdata>
	inline mcu_server::DataSender<Tdata> *CustomReceiver<Tdata>::clone() const {
		return new CustomReceiver(*this);
	}
}

#endif // CUSTOM_RECEIVER_HPP