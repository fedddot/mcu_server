#ifndef	SET_GPIO_TASK_HPP
#define	SET_GPIO_TASK_HPP

#include <memory>

#include "data.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"
#include "parser.hpp"

namespace server {
	template <class Tgpio_id>
	class SetGpioTask: public InventoryTask<Tgpio_id, Gpio> {
	public:
		using GpioInventory = Inventory<Tgpio_id, Gpio>;
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;
		using GpioStateParser = engine::Parser<Gpio::State(const engine::Data&)>;
		
		SetGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser, const GpioStateParser& gpio_state_parser);
		SetGpioTask(const SetGpioTask& other) = delete;
		SetGpioTask& operator=(const SetGpioTask& other) = delete;
		
		engine::Data *execute() override;
	private:
		std::unique_ptr<GpioIdParser> m_gpio_id_parser;
		std::unique_ptr<GpioStateParser> m_gpio_state_parser;
	};

	template <class Tgpio_id>
	inline SetGpioTask<Tgpio_id>::SetGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser, const GpioStateParser& gpio_state_parser): InventoryTask<Tgpio_id, Gpio>(inventory, task_data), m_gpio_id_parser(gpio_id_parser.clone()), m_gpio_state_parser(gpio_state_parser.clone()) {

	}

	template <class Tgpio_id>
	inline engine::Data *SetGpioTask<Tgpio_id>::execute() {
		auto gpio_id = m_gpio_id_parser->parse(this->access_data());
		auto gpio_state = m_gpio_state_parser->parse(this->access_data());
		Gpio::cast<Gpo>(*(this->access_inventory()->access(gpio_id))).set_state(gpio_state);
		engine::Object report;
		report.add("result", engine::Integer(0));
		return report.clone();
	}
}
#endif // SET_GPIO_TASK_HPP