#include <memory>

#include "gtest/gtest.h"

#include "gpo.hpp"
#include "stepper_motor.hpp"
#include "test_gpo.hpp"

using namespace manager;
using namespace manager_uts;

using ControlOutput = typename StepperMotor::ControlOutput;
using DirectionOutput = typename StepperMotor::DirectionOutput;
using GpoState = typename Gpo::State;

TestGpo s_ena, s_enb;
const StepperMotor::ControlOutputs s_control_outputs {
	{ControlOutput::ENA, &s_ena},
	{ControlOutput::ENB, &s_enb},
};

TestGpo s_a_top, s_a_btm, s_b_top, s_b_btm;
const StepperMotor::DirectionOutputs s_direction_outputs {
	{DirectionOutput::A_TOP, &s_a_top},
	{DirectionOutput::A_BTM, &s_a_btm},
	{DirectionOutput::B_TOP, &s_b_top},
	{DirectionOutput::B_BTM, &s_b_btm}
};

const StepperMotor::MotorStates s_states {
	{
		{DirectionOutput::A_TOP, GpoState::HIGH},
		{DirectionOutput::A_BTM, GpoState::LOW},
		{DirectionOutput::B_TOP, GpoState::LOW},
		{DirectionOutput::B_BTM, GpoState::LOW}
	},
	{
		{DirectionOutput::A_TOP, GpoState::LOW},
		{DirectionOutput::A_BTM, GpoState::LOW},
		{DirectionOutput::B_TOP, GpoState::HIGH},
		{DirectionOutput::B_BTM, GpoState::LOW}
	}
};

TEST(ut_stepper_motor, ctor_dtor_sanity) {
	// WHEN
	using StepperMotorUnqPtr = std::unique_ptr<StepperMotor>;
	StepperMotorUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = StepperMotorUnqPtr(new StepperMotor(s_control_outputs, s_direction_outputs, s_states)));
	ASSERT_NE(nullptr, instance_ptr);
	for (const auto& [output, state]: s_states[0]) {
		ASSERT_EQ(state, s_direction_outputs.at(output)->state());
	}
	for (const auto& output: {ControlOutput::ENA, ControlOutput::ENB}) {
		ASSERT_EQ(GpoState::LOW, s_control_outputs.at(output)->state());
	}

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_stepper_motor, enable_disable_is_enabled_step_sanity) {
	// WHEN
	StepperMotor instance(s_control_outputs, s_direction_outputs, s_states);

	// THEN
	// enable, enabled
	ASSERT_FALSE(instance.enabled());
	ASSERT_NO_THROW(instance.enable());
	ASSERT_TRUE(instance.enabled());
	ASSERT_EQ(GpoState::HIGH, s_ena.state());
	ASSERT_EQ(GpoState::HIGH, s_enb.state());

	// disable, enabled
	ASSERT_NO_THROW(instance.disable());
	ASSERT_FALSE(instance.enabled());
	ASSERT_EQ(GpoState::LOW, s_ena.state());
	ASSERT_EQ(GpoState::LOW, s_enb.state());

	// step
	ASSERT_NO_THROW(instance.step(StepperMotor::Direction::CW));
	for (const auto& [output, state]: s_states[1]) {
		ASSERT_EQ(state, s_direction_outputs.at(output)->state());
	}
	ASSERT_NO_THROW(instance.step(StepperMotor::Direction::CCW));
	for (const auto& [output, state]: s_states[0]) {
		ASSERT_EQ(state, s_direction_outputs.at(output)->state());
	}
}