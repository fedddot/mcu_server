#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "stepper_motor.hpp"
#include "test_gpo.hpp"
#include "test_platform.hpp"

using namespace mcu_platform;
using namespace mcu_platform_uts;
using namespace server_utl;

using GpoId = int;
using Shoulder = typename StepperMotor<int>::Shoulder;
using Shoulders = typename StepperMotor<int>::Shoulders;
using GpioState = typename Gpio::State;
using States = typename StepperMotor<int>::States;

TEST(ut_stepper_motor, ctor_dtor_sanity) {
	// GIVEN
	CustomCreator<Gpo *(const GpoId&)> gpo_ctor(
		[](const GpoId&) {
			return new TestGpo();
		}
	);
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
		},
		{
			{ Shoulder::IN0, GpioState::LOW },
			{ Shoulder::IN1, GpioState::HIGH },
			{ Shoulder::IN2, GpioState::LOW },
			{ Shoulder::IN3, GpioState::LOW }
		},
		{
			{ Shoulder::IN0, GpioState::LOW },
			{ Shoulder::IN1, GpioState::LOW },
			{ Shoulder::IN2, GpioState::LOW },
			{ Shoulder::IN3, GpioState::HIGH }
		}
	};
	TestPlatform<int> platform;

	// WHEN
	StepperMotor<int> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new StepperMotor<int>(
				shoulders,
				states,
				&platform
			)
		)
	);
	ASSERT_NO_THROW(instance_ptr->steps(StepperMotor<int>::Direction::CCW, 30, 30));
	ASSERT_NO_THROW(instance_ptr->steps(StepperMotor<int>::Direction::CW, 30, 30));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}