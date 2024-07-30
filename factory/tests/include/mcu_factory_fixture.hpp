#ifndef MCU_FACTORY_FIXTURE_HPP
#define MCU_FACTORY_FIXTURE_HPP

#include <functional>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "custom_creator.hpp"
#include "custom_parser.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "platform.hpp"
#include "test_platform.hpp"

namespace mcu_factory_uts {
	class McuFactoryFixture: public testing::Test {
	public:
		using McuData = std::string;
		using GpioId = int;
		using TestFactory = mcu_factory::McuFactory<McuData, GpioId>;
		using McuPlatform = mcu_platform::Platform<McuData, GpioId>;

		McuFactoryFixture(const std::string& msg_head = "test_msg_head", const std::string& msg_tail = "test_msg_tail", const std::size_t& max_buffer_size = 1000UL);
		McuFactoryFixture(const McuFactoryFixture&) = delete;
		McuFactoryFixture& operator=(const McuFactoryFixture&) = delete;
		
		McuPlatform *platform() const {
			return &m_platform;
		}

		const TestFactory::TaskTypeParser& task_type_parser() const {
			return *m_task_type_parser;
		}

		const TestFactory::GpioIdParser& gpio_id_parser() const {
			return *m_gpio_id_parser;
		}

		const TestFactory::GpioDirParser& gpio_dir_parser() const {
			return *m_gpio_dir_parser;
		}

		const TestFactory::GpioStateParser& gpio_state_parser() const {
			return *m_gpio_state_parser;
		}

		const TestFactory::TasksParser& tasks_parser() const {
			return *m_tasks_parser;
		}

		const TestFactory::DelayParser& delay_parser() const {
			return *m_delay_parser;
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

		mcu_server::Object delete_gpio_data(const GpioId& id) const {
			using namespace mcu_server;
			Object task_data;
			task_data.add("task_type", Integer(static_cast<int>(TestFactory::TaskType::DELETE_GPIO)));
			task_data.add("gpio_id", Integer(id));
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

		void set_sender(const std::function<void(const McuData&)>& send_function) {
			m_platform.set_sender(send_function);
		}

		std::string msg_head() const {
			return m_msg_head;
		}

		std::string msg_tail() const {
			return m_msg_tail;
		}
	private:
		
		const std::string m_msg_head;
		const std::string m_msg_tail;
		mutable mcu_platform_uts::TestPlatform m_platform;
		std::unique_ptr<TestFactory::TaskTypeParser> m_task_type_parser;
		std::unique_ptr<TestFactory::GpioIdParser> m_gpio_id_parser;
		std::unique_ptr<TestFactory::GpioDirParser> m_gpio_dir_parser;
		std::unique_ptr<TestFactory::GpioStateParser> m_gpio_state_parser;
		std::unique_ptr<TestFactory::TasksParser> m_tasks_parser;
		std::unique_ptr<TestFactory::DelayParser> m_delay_parser;
		std::unique_ptr<TestFactory::ResultReporter> m_result_reporter;
		std::unique_ptr<TestFactory::ResultStateReporter> m_result_state_reporter;
		std::unique_ptr<TestFactory::TasksResultsReporter> m_tasks_results_reporter;
	};

	inline McuFactoryFixture::McuFactoryFixture(const std::string& msg_head, const std::string& msg_tail, const std::size_t& max_buffer_size): m_msg_head(msg_head), m_msg_tail(msg_tail), m_platform(msg_head, msg_tail, max_buffer_size) {
		using namespace mcu_factory;
		using namespace mcu_server;
		using namespace mcu_server_utl;
		using namespace mcu_platform;
		using McuTaskType = typename TestFactory::TaskType;
		
		m_task_type_parser = std::unique_ptr<TestFactory::TaskTypeParser>(
			new CustomParser<McuTaskType(const Data&)>(
				[](const Data& data) {
					return static_cast<McuTaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
				}
			)
		);
		m_gpio_id_parser = std::unique_ptr<TestFactory::GpioIdParser>(
			new CustomParser<GpioId(const Data&)>(
				[](const Data& data) {
					return static_cast<GpioId>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get());
				}
			)
		);
		m_gpio_dir_parser = std::unique_ptr<TestFactory::GpioDirParser>(
			new CustomParser<Gpio::Direction(const Data&)>(
				[](const Data& data) {
					return static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_dir")).get());
				}
			)
		);
		m_gpio_state_parser = std::unique_ptr<TestFactory::GpioStateParser>(
			new CustomParser<Gpio::State(const Data&)>(
				[](const Data& data) {
					return static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
				}
			)
		);
		m_tasks_parser = std::unique_ptr<TestFactory::TasksParser>(
			new CustomParser<Array(const Data&)>(
				[](const Data& data) {
					return Array(Data::cast<Array>(Data::cast<Object>(data).access("tasks")));
				}
			)
		);
		m_delay_parser = std::unique_ptr<TestFactory::DelayParser>(
			new CustomParser<unsigned int(const Data&)>(
				[](const Data& data) {
					return static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("delay_ms")).get());
				}
			)
		);
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