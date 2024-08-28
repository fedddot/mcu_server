#ifndef FACTORY_FIXTURE_HPP
#define FACTORY_FIXTURE_HPP

#include "gtest/gtest.h"

#include "gpio_test_data_creator.hpp"
#include "stepper_motor_test_data_creator.hpp"
#include "task_creator.hpp"

namespace mcu_factory_uts {
	class FactoryFixture: public testing::Test {
	public:
		FactoryFixture() = default;
		FactoryFixture(const FactoryFixture&) = delete;
		FactoryFixture& operator=(const FactoryFixture&) = delete;

		const StepperMotorTestDataCreator& stepper_motor_data_ctor() const;
		const GpioTestDataCreator& gpio_data_ctor() const;
		const mcu_factory::TaskCreator& gpio_tasks_ctor() const;
		const mcu_factory::TaskCreator& stepper_motor_tasks_ctor() const;
	};
}

#endif