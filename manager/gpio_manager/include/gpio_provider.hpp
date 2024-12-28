#ifndef	GPIO_PROVIDER_HPP
#define	GPIO_PROVIDER_HPP

#include "gpio.hpp"
#include "provider.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioProvider: public mcu_app::Provider {
	public:
		virtual Gpio *create_gpio(const Tgpio_id& id, const Gpio::Direction& direction) const = 0;
	};
}

#endif // GPIO_PROVIDER_HPP