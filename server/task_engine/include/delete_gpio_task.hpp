#ifndef	DELETE_GPIO_TASK_HPP
#define	DELETE_GPIO_TASK_HPP

#include <memory>

#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"
#include "parser.hpp"

namespace server {
	template <class Tgpio_id>
	class DeleteGpioTask: public InventoryTask<Tgpio_id, Gpio> {
	public:
		using GpioInventory = Inventory<Tgpio_id, Gpio>;
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;
		
		DeleteGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser);
		DeleteGpioTask(const DeleteGpioTask& other) = delete;
		DeleteGpioTask& operator=(const DeleteGpioTask& other) = delete;
		
		engine::Data *execute() override;
	private:
		std::unique_ptr<GpioIdParser> m_gpio_id_parser;
	};

	template <class Tgpio_id>
	inline DeleteGpioTask<Tgpio_id>::DeleteGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser): InventoryTask<Tgpio_id, Gpio>(inventory, task_data), m_gpio_id_parser(gpio_id_parser.clone()) {

	}

	template <class Tgpio_id>
	inline engine::Data *DeleteGpioTask<Tgpio_id>::execute() {
		auto gpio_id = m_gpio_id_parser->parse(this->access_data());
		delete this->access_inventory()->pull(gpio_id);
		engine::Object report;
		report.add("result", engine::Integer(0));
		return report.clone();
	}
}
#endif // DELETE_GPIO_TASK_HPP