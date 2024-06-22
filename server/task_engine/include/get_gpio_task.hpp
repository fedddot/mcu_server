#ifndef	GET_GPIO_TASK_HPP
#define	GET_GPIO_TASK_HPP

#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "gpi.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"
#include "parser.hpp"

namespace server {
	template <class Tgpio_id>
	class GetGpioTask: public InventoryTask<Tgpio_id, Gpio> {
	public:
		using GpioInventory = Inventory<Tgpio_id, Gpio>;
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;
		
		GetGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser);
		GetGpioTask(const GetGpioTask& other) = delete;
		GetGpioTask& operator=(const GetGpioTask& other) = delete;
		
		engine::Data *execute() override;
	private:
		std::unique_ptr<GpioIdParser> m_gpio_id_parser;
	};

	template <class Tgpio_id>
	inline GetGpioTask<Tgpio_id>::GetGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser): InventoryTask<Tgpio_id, Gpio>(inventory, task_data), m_gpio_id_parser(gpio_id_parser.clone()) {

	}

	template <class Tgpio_id>
	inline engine::Data *GetGpioTask<Tgpio_id>::execute() {
		auto gpio_id = m_gpio_id_parser->parse(this->access_data());
		auto gpio_dir = this->access_inventory()->access(gpio_id)->direction();
		Gpio::State gpio_state(Gpio::State::LOW);
		switch (gpio_dir) {
		case Gpio::Direction::IN:
			gpio_state = Gpio::cast<Gpi>(*(this->access_inventory()->access(gpio_id))).state();
			break;
		case Gpio::Direction::OUT:
			gpio_state = Gpio::cast<Gpo>(*(this->access_inventory()->access(gpio_id))).state();
			break;
		default:
			throw std::runtime_error("unsupported Gpio direction");
		}
		engine::Object report;
		report.add("result", engine::Integer(0));
		report.add("gpio_state", engine::Integer(static_cast<int>(gpio_state)));
		return report.clone();
	}
}
#endif // GET_GPIO_TASK_HPP