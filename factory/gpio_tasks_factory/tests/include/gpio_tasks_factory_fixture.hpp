#ifndef GPIO_TASKS_FACTORY_FIXTURE_HPP
#define GPIO_TASKS_FACTORY_FIXTURE_HPP

#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "data.hpp"
#include "default_gpio_tasks_data_retriever.hpp"
#include "gpio.hpp"
#include "gpio_tasks_factory.hpp"
#include "gpio_test_data_creator.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "test_platform.hpp"

namespace mcu_factory_uts {
	class GpioTasksFactoryFixture: public testing::Test {
	public:
		using GpioId = int;
		using TestFactory = mcu_factory::GpioTasksFactory<GpioId>;
		using GpioInventory = typename TestFactory::GpioInventory;
		using GpioState = typename TestFactory::GpioState;
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using Retriever = typename TestFactory::DataRetriever;
		using FactoryPlatform = typename TestFactory::FactoryPlatform;
		
		GpioTasksFactoryFixture();
		GpioTasksFactoryFixture(const GpioTasksFactoryFixture&) = delete;
		GpioTasksFactoryFixture& operator=(const GpioTasksFactoryFixture&) = delete;
		
		GpioInventory *inventory() const {
			return &m_inventory;
		}
		
		const FactoryPlatform *platform() const {
			return &m_platform;
		}

		const Retriever& retriever() const {
			return m_retriever;
		}

		const TestFactory::ResultReporter& result_reporter() const {
			return *m_result_reporter;
		}

		const TestFactory::ResultStateReporter& result_state_reporter() const {
			return *m_result_state_reporter;
		}

		const GpioTestDataCreator& test_data_ctor() const {
			return m_test_data_ctor;
		}

	private:
		mutable GpioInventory m_inventory;
		mcu_platform_uts::TestPlatform<GpioId> m_platform;
		mcu_factory::DefaultGpioTasksDataRetriever<TestFactory::TaskType, GpioId> m_retriever;
		std::unique_ptr<TestFactory::ResultReporter> m_result_reporter;
		std::unique_ptr<TestFactory::ResultStateReporter> m_result_state_reporter;
		GpioTestDataCreator m_test_data_ctor;
	};

	inline GpioTasksFactoryFixture::GpioTasksFactoryFixture() {
		using namespace mcu_factory;
		using namespace server;
		using namespace server_utl;
		using namespace mcu_platform;
		using McuTaskType = typename TestFactory::TaskType;
		
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
	}
}

#endif