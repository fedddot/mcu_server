#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include "data.hpp"
#include "gpi.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory_manager.hpp"

#include "object.hpp"
#include "resource.hpp"
#include <stdexcept>

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
		static typename Gpio::State read_gpio_state(const Gpio& gpio);
	};

	inline GpioManager::GpioManager(const Creator& creator): InventoryManager<Gpio>(creator, Duplicator(GpioManager::duplicate), Reader(GpioManager::read_gpio), Writer(GpioManager::write_gpio)) {

	}

	inline server::Resource *GpioManager::clone() const {
		return new GpioManager(*this);
	}

	inline Gpio *GpioManager::duplicate(const Gpio& instance) {
		return instance.clone();
	}
	
	inline server::Object GpioManager::read_gpio(const Gpio& gpio) {
		using namespace server;
		Object gpio_data;
		gpio_data.add("dir", Integer(static_cast<int>(gpio.direction())));
		gpio_data.add("state", Integer(static_cast<int>(read_gpio_state(gpio))));
		return gpio_data;
	}

	inline void GpioManager::write_gpio(Gpio *gpio_ptr, const server::Data& data) {
		using namespace server;
		const auto state = static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("state")).get());
		Gpio::cast<Gpo>(*gpio_ptr).set_state(state);
	}

	inline typename Gpio::State GpioManager::read_gpio_state(const Gpio& gpio) {
		switch (gpio.direction()) {
		case Gpio::Direction::IN:
			return Gpio::cast<Gpi>(gpio).state();
		case Gpio::Direction::OUT:
			return Gpio::cast<Gpo>(gpio).state();
		default:
			throw std::invalid_argument("unsupported Gpio type");
		}
	}
}

#endif // GPIO_MANAGER_HPP