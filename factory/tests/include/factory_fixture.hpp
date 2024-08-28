#ifndef FACTORY_FIXTURE_HPP
#define FACTORY_FIXTURE_HPP

#include "gtest/gtest.h"
#include <memory>

#include "custom_creator.hpp"
#include "data.hpp"
#include "default_gpio_tasks_data_retriever.hpp"
#include "default_stepper_motor_tasks_data_retriever.hpp"
#include "gpio.hpp"
#include "gpio_tasks_factory.hpp"
#include "gpio_test_data_creator.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_tasks_factory.hpp"
#include "stepper_motor_test_data_creator.hpp"
#include "task_creator.hpp"
#include "test_platform.hpp"

namespace mcu_factory_uts {
	class FactoryFixture: public testing::Test {
	public:
		using GpioId = int;
		using StepperId = int;
		
		FactoryFixture();
		FactoryFixture(const FactoryFixture&) = delete;
		FactoryFixture& operator=(const FactoryFixture&) = delete;

		const StepperMotorTestDataCreator& stepper_motor_data_ctor() const {
			return m_stepper_motor_data_ctor;
		}
		const GpioTestDataCreator& gpio_data_ctor() const {
			return m_gpio_data_ctor;
		}
		const mcu_factory::TaskCreator& gpio_tasks_ctor() const {
			return *m_gpio_tasks_ctor;
		}
		const mcu_factory::TaskCreator& stepper_motor_tasks_ctor() const {
			return *m_stepper_motor_tasks_ctor;
		}
	private:
		using GpioInventory = mcu_platform::Inventory<GpioId, mcu_platform::Gpio>;
		using StepperMotorInventory = mcu_platform::Inventory<StepperId, mcu_platform::StepperMotor<GpioId>>;
		GpioInventory m_gpio_inventory;
		StepperMotorInventory m_stepper_motor_inventory;
		mcu_platform_uts::TestPlatform<GpioId> m_platform;
		server_utl::CustomCreator<server::Data *(int)> m_report_ctor;
		server_utl::CustomCreator<server::Data *(int, const mcu_platform::Gpio::State&)> m_state_report_ctor;
		StepperMotorTestDataCreator m_stepper_motor_data_ctor;
		GpioTestDataCreator m_gpio_data_ctor;
		std::unique_ptr<mcu_factory::TaskCreator> m_gpio_tasks_ctor;
		std::unique_ptr<mcu_factory::TaskCreator> m_stepper_motor_tasks_ctor;
	};

	inline FactoryFixture::FactoryFixture():
		m_report_ctor(
			[](int result) {
				server::Object report;
				report.add("result", server::Integer(result));
				return report.clone();
			}
		),
		m_state_report_ctor(
			[](int result, const mcu_platform::Gpio::State& state) {
				server::Object report;
				report.add("result", server::Integer(result));
				report.add("gpio_state", server::Integer(static_cast<int>(state)));
				return report.clone();
			}
		) {
			m_gpio_tasks_ctor = std::unique_ptr<mcu_factory::TaskCreator>(
				new mcu_factory::GpioTasksFactory<GpioId>(
					&m_gpio_inventory,
					&m_platform,
					mcu_factory::DefaultGpioTasksDataRetriever<GpioId>(),
					m_report_ctor,
					m_state_report_ctor
				)
			);
			m_stepper_motor_tasks_ctor = std::unique_ptr<mcu_factory::TaskCreator>(
				new mcu_factory::StepperMotorTasksFactory<StepperId, GpioId>(
					&m_stepper_motor_inventory,
					&m_platform,
					mcu_factory::DefaultStepperMotorDataRetriever<GpioId, StepperId>(),
					m_report_ctor
				)
			);
		}
}

#endif