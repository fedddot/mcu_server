#ifndef	GPIO_CREATE_REQUEST_HPP
#define	GPIO_CREATE_REQUEST_HPP

#include "gpio.hpp"
#include "gpio_request.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioCreateRequest: public GpioRequest<Tgpio_id> {
	public:
		GpioCreateRequest(const Tgpio_id& id, const Gpio::Direction& direction);
		GpioCreateRequest(const GpioCreateRequest&) = default;
		GpioCreateRequest& operator=(const GpioCreateRequest&) = default;

		Gpio::Direction direction() const;
	private:
		Gpio::Direction m_direction;
	};

	template <typename Tgpio_id>
	inline GpioCreateRequest<Tgpio_id>::GpioCreateRequest(const Tgpio_id& id, const Gpio::Direction& direction): GpioRequest<Tgpio_id>(GpioRequest<Tgpio_id>::Operation::CREATE, id), m_direction(direction) {

	}

	template <typename Tgpio_id>
	inline typename Gpio::Direction GpioCreateRequest<Tgpio_id>::direction() const {
		return m_direction;
	}
}

#endif // GPIO_CREATE_REQUEST_HPP