#ifndef	CREATE_GPIO_TASK_HPP
#define	CREATE_GPIO_TASK_HPP

#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"
#include "parser.hpp"

namespace server {
	template <class Tgpio_id>
	class CreateGpioTask: public InventoryTask<Tgpio_id, Gpio> {
	public:
		using GpioInventory = Inventory<Tgpio_id, Gpio>;
		using GpioIdParser = engine::Parser<Tgpio_id(const engine::Data&)>;
		using GpioDirectionParser = engine::Parser<Gpio::Direction(const engine::Data&)>;
		using GpioCreator = engine::Creator<Gpio *(const Tgpio_id&, const Gpio::Direction&)>;

		CreateGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser, const GpioDirectionParser& gpio_dir_parser, const GpioCreator& gpio_creator);
		CreateGpioTask(const CreateGpioTask& other) = delete;
		CreateGpioTask& operator=(const CreateGpioTask& other) = delete;
		
		engine::Data *execute() override;
	private:
		std::unique_ptr<GpioIdParser> m_gpio_id_parser;
		std::unique_ptr<GpioDirectionParser> m_gpio_dir_parser;
		std::unique_ptr<GpioCreator> m_gpio_creator;
	};

	template <class Tgpio_id>
	inline CreateGpioTask<Tgpio_id>::CreateGpioTask(GpioInventory *inventory, const engine::Data& task_data, const GpioIdParser& gpio_id_parser, const GpioDirectionParser& gpio_dir_parser, const GpioCreator& gpio_creator): InventoryTask<Tgpio_id, Gpio>(inventory, task_data), m_gpio_id_parser(gpio_id_parser.clone()), m_gpio_dir_parser(gpio_dir_parser.clone()), m_gpio_creator(gpio_creator.clone()) {

	}

	template <class Tgpio_id>
	inline engine::Data *CreateGpioTask<Tgpio_id>::execute() {
		auto gpio_id = m_gpio_id_parser->parse(this->access_data());
		auto gpio_dir = m_gpio_dir_parser->parse(this->access_data());
		this->access_inventory()->put(gpio_id, m_gpio_creator->create(gpio_id, gpio_dir));
		engine::Object report;
		report.add("result", engine::Integer(0));
		return report.clone();
	}
}
#endif // CREATE_GPIO_TASK_HPP