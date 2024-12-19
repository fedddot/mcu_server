#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include "data.hpp"
#include "gpi.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_manager.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"

namespace manager {
	class GpioManager: public InventoryManager<Gpio> {
	public:
		using GpioCreator = typename InventoryManager<Gpio>::ItemCreator;
		GpioManager(Inventory<server::ResourceId, Gpio> *gpio_inventory, const GpioCreator& gpio_creator);
		GpioManager(const GpioManager& other) = delete;
		GpioManager& operator=(const GpioManager&) = delete;
	private:
		static server::Object read_gpio(const Gpio& gpio);
		static void write_gpio(Gpio *gpio, const server::Data& update_cfg);
	};
	
	inline GpioManager::GpioManager(Inventory<server::ResourceId, Gpio> *gpio_inventory, const GpioCreator& gpio_creator): InventoryManager<Gpio>(gpio_inventory, gpio_creator, read_gpio, write_gpio) {

	}

	inline server::Object GpioManager::read_gpio(const Gpio& gpio) {
		using namespace server;
		const auto gpio_direction(gpio.direction());
		auto gpio_state(Gpio::State::LOW);
		switch (gpio_direction) {
		case Gpio::Direction::IN:
			gpio_state = Gpio::cast<Gpi>(gpio).state();
			break;
		case Gpio::Direction::OUT:
			gpio_state = Gpio::cast<Gpo>(gpio).state();
			break;
		default:
			throw ServerException(ResponseCode::UNSPECIFIED, "unsupported gpio direction");
		}
		Object gpio_data;
		gpio_data.add("direction", Integer(static_cast<int>(gpio_direction)));
		gpio_data.add("state", Integer(static_cast<int>(gpio_state)));
		return gpio_data;
	}
	
	inline void GpioManager::write_gpio(Gpio *gpio, const server::Data& update_cfg) {
		using namespace server;
		if (Gpio::Direction::OUT != gpio->direction()) {
			throw ServerException(ResponseCode::BAD_REQUEST, "requested gpio is not an output");
		}
		const auto state(static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(update_cfg).access("state")).get()));
		Gpio::cast<Gpo>(*gpio).set_state(state);
	}
}

#endif // GPIO_MANAGER_HPP