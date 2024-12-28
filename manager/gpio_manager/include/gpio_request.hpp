#ifndef	GPIO_REQUEST_HPP
#define	GPIO_REQUEST_HPP

#include "gpio.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioRequest {
	public:
		enum class Operation: int {
			CREATE,
			READ,
			WRITE,
			DELETE
		};
		GpioRequest(const Operation& operation);
		GpioRequest(const GpioRequest&) = default;
		GpioRequest& operator=(const GpioRequest&) = default;
		virtual ~GpioRequest() noexcept = default;

		Operation operation() const;

		Tgpio_id id() const;
		void set_id(const Tgpio_id& id);

		Gpio::Direction direction() const;
		void set_direction(const Gpio::Direction& direction);

		Gpio::State state() const;
		void set_state(const Gpio::State& state);
	private:
		Operation m_operation;
		Tgpio_id m_id;
		Gpio::Direction m_direction;
		Gpio::State m_state;
	};

	template <typename Tgpio_id>
	inline GpioRequest<Tgpio_id>::GpioRequest(const Operation& operation): m_operation(operation) {

	}

	template <typename Tgpio_id>
	inline typename GpioRequest<Tgpio_id>::Operation GpioRequest<Tgpio_id>::operation() const {
		return m_operation;
	}

	template <typename Tgpio_id>
	inline Tgpio_id GpioRequest<Tgpio_id>::id() const {
		return m_id;
	}

	template <typename Tgpio_id>
	inline void GpioRequest<Tgpio_id>::set_id(const Tgpio_id& id) {
		m_id = id;
	}

	template <typename Tgpio_id>
	inline typename Gpio::Direction GpioRequest<Tgpio_id>::direction() const {
		return m_direction;
	}

	template <typename Tgpio_id>
	inline void GpioRequest<Tgpio_id>::set_direction(const Gpio::Direction& direction) {
		m_direction = direction;
	}

	template <typename Tgpio_id>
	inline typename Gpio::State GpioRequest<Tgpio_id>::state() const {
		return m_state;
	}

	template <typename Tgpio_id>
	inline void GpioRequest<Tgpio_id>::set_state(const Gpio::State& state) {
		m_state = state;
	}
}

#endif // GPIO_REQUEST_HPP