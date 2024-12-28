#ifndef	GPIO_CREATE_REQUEST_HPP
#define	GPIO_CREATE_REQUEST_HPP

#include "gpio.hpp"
#include "gpio_request.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioCreateRequest: public GpioRequest<Tgpio_id> {
	public:
		using Operation = typename GpioRequest<Tgpio_id>::Operation;
		GpioCreateRequest(const Tgpio_id& id, const Gpio::Direction& direction);
		GpioCreateRequest(const GpioCreateRequest&) = default;
		GpioCreateRequest& operator=(const GpioCreateRequest&) = default;
		Operation operation() const override;
		virtual Tgpio_id id() const override;

		Gpio::Direction direction() const;
	private:
		Tgpio_id m_id;
		Gpio::Direction m_direction;
	};

	template <typename Tgpio_id>
	inline GpioCreateRequest<Tgpio_id>::GpioCreateRequest(const Tgpio_id& id, const Gpio::Direction& direction): m_id(id), m_direction(direction) {

	}

	template <typename Tgpio_id>
	inline typename GpioCreateRequest<Tgpio_id>::Operation GpioCreateRequest<Tgpio_id>::operation() const {
		return Operation::CREATE;
	}

	template <typename Tgpio_id>
	inline Tgpio_id GpioCreateRequest<Tgpio_id>::id() const {
		return m_id;
	}

	template <typename Tgpio_id>
	inline typename Gpio::Direction GpioCreateRequest<Tgpio_id>::direction() const {
		return m_direction;
	}
}

#endif // GPIO_CREATE_REQUEST_HPP