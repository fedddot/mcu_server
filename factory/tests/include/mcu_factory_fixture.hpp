#ifndef MCU_FACTORY_FIXTURE_HPP
#define MCU_FACTORY_FIXTURE_HPP

#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "default_mcu_factory_parsers.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "mcu_factory_parsers.hpp"
#include "object.hpp"
#include "string.hpp"
#include "test_platform.hpp"

namespace mcu_factory_uts {
	class McuFactoryFixture: public testing::Test {
	public:
		using GpioId = int;
		using TaskId = std::string;
		using TestFactory = mcu_factory::McuFactory<GpioId, TaskId>;
		using TestParsers = mcu_factory::McuFactoryParsers<GpioId, TaskId, TestFactory::TaskType>;

		McuFactoryFixture();
		McuFactoryFixture(const McuFactoryFixture&) = delete;
		McuFactoryFixture& operator=(const McuFactoryFixture&) = delete;
		
		mcu_platform_uts::TestPlatform *platform() const {
			return &m_platform;
		}

		const TestParsers& parsers() const {
			return *m_parsers;
		}

		const TestFactory::ResultReporter& result_reporter() const {
			return *m_result_reporter;
		}

		const TestFactory::ResultStateReporter& result_state_reporter() const {
			return *m_result_state_reporter;
		}

		const TestFactory::TasksResultsReporter& tasks_results_reporter() const {
			return *m_tasks_results_reporter;
		}

		mcu_server::Object create_gpio_data(const GpioId& id, const mcu_platform::Gpio::Direction& dir) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::CREATE_GPIO)));
			task_data.add("gpio_id", Integer(id));
			task_data.add("gpio_dir", Integer(static_cast<int>(dir)));
			return task_data;
		}

		mcu_server::Object create_persistent_task_data(const TaskId& id, const mcu_server::Data& pers_task_data) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::CREATE_PERSISTENT_TASK)));
			task_data.add("task_id", String(id));
			task_data.add("task_data", pers_task_data);
			return task_data;
		}

		mcu_server::Object set_gpio_data(const GpioId& id, const mcu_platform::Gpio::State& state) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::SET_GPIO)));
			task_data.add("gpio_id", Integer(id));
			task_data.add("gpio_state", Integer(static_cast<int>(state)));
			return task_data;
		}

		mcu_server::Object get_gpio_data(const GpioId& id) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::GET_GPIO)));
			task_data.add("gpio_id", Integer(id));
			return task_data;
		}

		mcu_server::Object execute_persistent_task_data(const TaskId& id) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::EXECUTE_PERSISTENT_TASK)));
			task_data.add("task_id", String(id));
			return task_data;
		}

		mcu_server::Object delete_gpio_data(const GpioId& id) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::DELETE_GPIO)));
			task_data.add("gpio_id", Integer(id));
			return task_data;
		}

		mcu_server::Object delete_persistent_task_data(const TaskId& id) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::DELETE_PERSISTENT_TASK)));
			task_data.add("task_id", String(id));
			return task_data;
		}

		mcu_server::Object sequence_data(const GpioId& id) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::SEQUENCE)));
			Array tasks;
			tasks.push_back(create_gpio_data(id, mcu_platform::Gpio::Direction::OUT));
			tasks.push_back(delay_data(100));
			tasks.push_back(set_gpio_data(id, mcu_platform::Gpio::State::HIGH));
			tasks.push_back(delay_data(100));
			tasks.push_back(delete_gpio_data(id));
			task_data.add("tasks", tasks);
			return task_data;
		}

		mcu_server::Object delay_data(int delay_ms) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::DELAY)));
			task_data.add("delay_ms", Integer(delay_ms));
			return task_data;
		}

	private:
		mutable mcu_platform_uts::TestPlatform m_platform;
		std::unique_ptr<TestParsers> m_parsers;
		std::unique_ptr<TestFactory::ResultReporter> m_result_reporter;
		std::unique_ptr<TestFactory::ResultStateReporter> m_result_state_reporter;
		std::unique_ptr<TestFactory::TasksResultsReporter> m_tasks_results_reporter;
	};

	inline McuFactoryFixture::McuFactoryFixture() {
		using namespace mcu_factory;
		using namespace mcu_server;
		using namespace mcu_server_utl;
		using namespace mcu_platform;
		using McuTaskType = typename TestFactory::TaskType;
		
		m_parsers = std::unique_ptr<TestParsers>(new DefaultMcuFactoryParsers<GpioId, TaskId, McuTaskType>());

		m_result_reporter = std::unique_ptr<TestFactory::ResultReporter>(
			new CustomCreator<Data *(int)>(
				[](int result) {
					Object report;
					report.add("result", Integer(result));
					return report.clone();
				}
			)
		);
		m_result_state_reporter = std::unique_ptr<TestFactory::ResultStateReporter>(
			new CustomCreator<Data *(int, const Gpio::State&)>(
				[](int result, const Gpio::State& state) {
					Object report;
					report.add("result", Integer(result));
					report.add("gpio_state", Integer(static_cast<int>(state)));
					return report.clone();
				}
			)
		);
		m_tasks_results_reporter = std::unique_ptr<TestFactory::TasksResultsReporter>(
			new CustomCreator<Data *(const Array&)>(
				[](const Array& data) {
					Object report;
					report.add("result", Integer(0));
					report.add("results", data);
					return report.clone();
				}
			)
		);
	}
}

#endif