#ifndef GPIO_TEST_DATA_CREATOR_HPP
#define GPIO_TEST_DATA_CREATOR_HPP

#include <string>

#include "gpio.hpp"
#include "gpio_tasks_factory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"

namespace mcu_factory_uts {
	class GpioTestDataCreator {
	public:
		using GpioId = int;
		using TestFactory = mcu_factory::GpioTasksFactory<GpioId>;
		
		GpioTestDataCreator() = default;
		GpioTestDataCreator(const GpioTestDataCreator&) = delete;
		GpioTestDataCreator& operator=(const GpioTestDataCreator&) = delete;

		server::Object create_gpio_data(const GpioId& id, const mcu_platform::Gpio::Direction& dir) const {
			using namespace server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::CREATE_GPIO)));
			task_data.add("gpio_id", Integer(id));
			task_data.add("gpio_dir", Integer(static_cast<int>(dir)));
			task_data.add("domain", String("gpio_tasks"));
			return task_data;
		}

		server::Object set_gpio_data(const GpioId& id, const mcu_platform::Gpio::State& state) const {
			using namespace server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::SET_GPIO)));
			task_data.add("gpio_id", Integer(id));
			task_data.add("gpio_state", Integer(static_cast<int>(state)));
			task_data.add("domain", String("gpio_tasks"));
			return task_data;
		}

		server::Object get_gpio_data(const GpioId& id) const {
			using namespace server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::GET_GPIO)));
			task_data.add("gpio_id", Integer(id));
			task_data.add("domain", String("gpio_tasks"));
			return task_data;
		}

		server::Object delete_gpio_data(const GpioId& id) const {
			using namespace server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::DELETE_GPIO)));
			task_data.add("gpio_id", Integer(id));
			task_data.add("domain", String("gpio_tasks"));
			return task_data;
		}
	};
}

#endif