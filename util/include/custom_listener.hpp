#ifndef	CUSTOM_LISTENER_HPP
#define	CUSTOM_LISTENER_HPP

#include <stdexcept>
#include <functional>

#include "listener.hpp"

namespace mcu_server_utl {
	
	template <typename Tevent>
	class CustomListener: public mcu_server::Listener<Tevent> {
	public:
		using ListenFunction = std::function<void(const Tevent&)>;
		
		CustomListener(const ListenFunction& listen_function);
		CustomListener(const CustomListener& other) = default;
		CustomListener& operator=(const CustomListener& other) = default;
				
		void on_event(const Tevent& data) override;
		mcu_server::Listener<Tevent> *clone() override;
	private:
		ListenFunction m_listen_function;
	};

	template <typename Tevent>
	inline CustomListener<Tevent>::CustomListener(const ListenFunction& listen_function): m_listen_function(listen_function) {
		if (!m_listen_function) {
			throw std::invalid_argument("invalid listen action received");
		}
	}

	template <typename Tevent>
	inline void CustomListener<Tevent>::on_event(const Tevent& data) {
		m_listen_function(data);
	}
}

#endif // CUSTOM_LISTENER_HPP