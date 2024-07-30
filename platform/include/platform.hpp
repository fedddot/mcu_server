#ifndef	PLATFORM_HPP
#define	PLATFORM_HPP

#include "gpio.hpp"
#include "inventory.hpp"
#include "message_receiver.hpp"
#include "message_sender.hpp"

namespace mcu_platform {
	template <typename Tdata, typename Tgpio_id>
	class Platform {
	public:
		virtual ~Platform() noexcept = default;
		
		virtual MessageReceiver<Tdata> *message_receiver() const = 0;
		virtual MessageSender<Tdata> *message_sender() const = 0;

		virtual void delay(unsigned int timeout_ms) const = 0;
		virtual Gpio *create_gpio(const Tgpio_id& id, const Gpio::Direction& dir) const = 0;

		virtual Inventory<Tgpio_id, Gpio> *gpio_inventory() const = 0;
	};
}
#endif // PLATFORM_HPP