#ifndef	GPIO_PROVIDER_HPP
#define	GPIO_PROVIDER_HPP

#include "gpio_data.hpp"
#include "gpio.hpp"
#include "provider.hpp"

namespace manager {
	class GpioProvider: public mcu_app::Provider {
	public:
		virtual Gpio *create_gpio(const GpioData& data) const = 0;
	};
}

#endif // GPIO_PROVIDER_HPP