#ifndef	PLATFORM_HPP
#define	PLATFORM_HPP

#include "gpio.hpp"

namespace mcu_platform {
	template <typename Tgpio_id>
	class Platform {
	public:
		virtual ~Platform() noexcept = default;

		virtual void delay(const unsigned int delay_ms) const = 0;
		virtual Gpio *create_gpio(const Tgpio_id& id, const Gpio::Direction& dir) const = 0;
	};
}
#endif // PLATFORM_HPP