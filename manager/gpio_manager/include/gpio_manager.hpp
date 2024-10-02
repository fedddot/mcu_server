#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include "gpio.hpp"
#include "inventory_manager.hpp"

#include "object.hpp"
#include "resource.hpp"

namespace manager {
	
	class GpioManager: public InventoryManager<Gpio> {
	public:
		GpioManager(const Creator& creator);
		GpioManager(const GpioManager& other) = default;
		GpioManager& operator=(const GpioManager&) = delete;
		
		server::Resource *clone() const override;
	private:
		static Gpio *duplicate(const Gpio& instance);
		static server::Object read_gpio(const Gpio& gpio);
		static void write_gpio(Gpio *gpio_ptr, const server::Data& data);
	};

	inline GpioManager::GpioManager(const Creator& creator): InventoryManager<Gpio>(creator, Duplicator(GpioManager::duplicate), Reader(GpioManager::read_gpio), Writer(GpioManager::write_gpio)) {

	}

	inline server::Resource *GpioManager::clone() const {
		return new GpioManager(*this);
	}
}

#endif // GPIO_MANAGER_HPP