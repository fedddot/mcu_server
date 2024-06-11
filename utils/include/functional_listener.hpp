#ifndef	FUNCTIONAL_LISTENER_HPP
#define	FUNCTIONAL_LISTENER_HPP

#include <stdexcept>
#include <functional>

#include "listener.hpp"

namespace mcu_control_utl {
	template <typename... Args>
	class FunctionalListener: public mcu_control::Listener<Args...> {
	public:
		using OnEventFunction = std::function<void(Args...)>;
		
		FunctionalListener(const OnEventFunction& on_event_function);
		FunctionalListener(const FunctionalListener& other) = default;
		FunctionalListener& operator=(const FunctionalListener& other) = default;
				
		void on_event(Args... args) override;
		mcu_control::Listener<Args...> *clone() const override;
	private:
		OnEventFunction m_on_event_function;
	};

	template <typename... Args>
	inline FunctionalListener<Args...>::FunctionalListener(const OnEventFunction& on_event_function): m_on_event_function(on_event_function) {
		if (!m_on_event_function) {
			throw std::invalid_argument("invalid create action received");
		}
	}

	template <typename... Args>
	inline void FunctionalListener<Args...>::on_event(Args... args) {
		return m_on_event_function(std::forward<Args>(args)...);
	}

	template <typename... Args>
	inline mcu_control::Listener<Args...> *FunctionalListener<Args...>::clone() const {
		return new FunctionalListener(*this);
	}
}

#endif // FUNCTIONAL_LISTENER_HPP