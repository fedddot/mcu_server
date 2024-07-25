#ifndef	CUSTOM_SENDER_HPP
#define	CUSTOM_SENDER_HPP

#include <stdexcept>
#include <functional>

#include "message_sender.hpp"

namespace mcu_server_utl {
	
	template <typename Tdata>
	class CustomSender: public mcu_server::DataSender<Tdata> {
	public:
		using SendFunction = std::function<void(const Tdata&)>;
		
		CustomSender(const SendFunction& send_function);
		CustomSender(const CustomSender& other) = default;
		CustomSender& operator=(const CustomSender& other) = default;
				
		void send(const Tdata& data) const override;
	private:
		SendFunction m_send_function;
	};

	template <typename Tdata>
	inline CustomSender<Tdata>::CustomSender(const SendFunction& send_function): m_send_function(send_function) {
		if (!m_send_function) {
			throw std::invalid_argument("invalid send action received");
		}
	}

	template <typename Tdata>
	inline void CustomSender<Tdata>::send(const Tdata& data) const {
		m_send_function(data);
	}
}

#endif // CUSTOM_SENDER_HPP