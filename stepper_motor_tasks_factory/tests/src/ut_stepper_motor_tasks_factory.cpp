#include "gtest/gtest.h"
#include <memory>

#include "custom_creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor_tasks_factory_fixture.hpp"
#include "task.hpp"
#include "test_delay.hpp"
#include "test_gpo.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;
using namespace mcu_factory;
using namespace mcu_factory_uts;

TEST_F(StepperMotorTasksFactoryFixture, ctor_dtor_sanity) {
	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(
				inventory(),
				task_type_parser(),
				stepper_id_parser(),
				states_parser(),
				shoulders_parser(),
				CustomCreator<mcu_platform::Gpo *(const GpioId&)>(
					[](const GpioId& gpio_id) {
						return new mcu_platform_uts::TestGpo();
					}
				),
				mcu_platform_uts::TestDelay(),
				CustomCreator<Data *(int)>(
					[](int result) {
						Object report;
						report.add("result", Integer(result));
						return report.clone();
					} 
				)
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new TestFactory(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}

TEST_F(StepperMotorTasksFactoryFixture, create_sanity) {
	// GIVEN
	using GpioState = typename mcu_platform::Gpio::State;
	Shoulders shoulders {
		{ Shoulder::IN0, 10 },
		{ Shoulder::IN1, 11 },
		{ Shoulder::IN2, 12 },
		{ Shoulder::IN3, 13 }
	};
	States states {
		{
			{ Shoulder::IN0, GpioState::HIGH },
			{ Shoulder::IN1, GpioState::LOW },
			{ Shoulder::IN2, GpioState::LOW },
			{ Shoulder::IN3, GpioState::LOW }
		},
		{
			{ Shoulder::IN0, GpioState::LOW },
			{ Shoulder::IN1, GpioState::LOW },
			{ Shoulder::IN2, GpioState::HIGH },
			{ Shoulder::IN3, GpioState::LOW }
		}
	};
	const StepperId id(12);

	// WHEN
	TestFactory instance(
		inventory(),
		task_type_parser(),
		stepper_id_parser(),
		states_parser(),
		shoulders_parser(),
		CustomCreator<mcu_platform::Gpo *(const GpioId&)>(
			[](const GpioId& gpio_id) {
				return new mcu_platform_uts::TestGpo();
			}
		),
		mcu_platform_uts::TestDelay(),
		CustomCreator<Data *(int)>(
			[](int result) {
				Object report;
				report.add("result", Integer(result));
				return report.clone();
			} 
		)
	);
	using Task = Task<Data *(void)>;
	std::unique_ptr<Task> task_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			task_ptr = std::unique_ptr<Task>(instance.create(create_data(id, shoulders, states)))
		)
	);
	ASSERT_NE(nullptr, task_ptr);
}