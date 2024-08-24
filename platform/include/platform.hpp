#ifndef	PLATFORM_HPP
#define	PLATFORM_HPP

#include "data.hpp"
#include "delay.hpp"
#include "gpio.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_platform {
	template <typename Tgpio_id, typename Ttask_id>
	class Platform {
	public:
		using PersistentTask = server::Task<server::Data *(void)>;

		virtual ~Platform() noexcept = default;

		virtual Delay *create_delay() const = 0;
		virtual Gpio *create_gpio(const Tgpio_id& id, const Gpio::Direction& dir) const = 0;
		virtual Inventory<Tgpio_id, Gpio> *gpio_inventory() const = 0;
		virtual Inventory<Ttask_id, PersistentTask> *task_inventory() const = 0;
	};
}
#endif // PLATFORM_HPP