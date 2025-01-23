#include <chrono>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>

#include "gtest/gtest.h"
#include <thread>

#include "stepper_motor_manager.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "test_stepper_motor.hpp"

using namespace manager;
using namespace manager_tests;

using StepperCreateConfig = std::string;

TEST(ut_stepper_motor_manager, sanity) {
	// GIVEN
	const auto test_motor_id = std::string("test_motor");
	const auto test_motor_create_cfg = StepperCreateConfig("test_motor_create_cfg");
	auto test_create_request = StepperMotorRequest<StepperCreateConfig>(
		StepperMotorRequest<StepperCreateConfig>::Type::CREATE_STEPPER,
		test_motor_id
	);
	test_create_request.set_create_config(test_motor_create_cfg);

	auto test_delete_request = StepperMotorRequest<StepperCreateConfig>(
		StepperMotorRequest<StepperCreateConfig>::Type::DELETE_STEPPER,
		test_motor_id
	);

	const auto test_direction = StepperMotor::Direction::CW;
	const auto test_steps_num = std::size_t(20);
	const auto test_step_duration = std::size_t(50);
	auto test_steps_request = StepperMotorRequest<StepperCreateConfig>(StepperMotorRequest<StepperCreateConfig>::Type::STEPS, test_motor_id);
	test_steps_request.set_steps(
		StepperMotorRequest<StepperCreateConfig>::Steps {
			.direction = test_direction,
			.steps_number = test_steps_num,
			.step_duration = test_step_duration
		}
	);
	const auto motor_ctor = [](const StepperCreateConfig& cfg) {
		return new TestStepperMotor(
			[](const StepperMotor::Direction& direction) {
				const auto dir_to_str_map = std::map<StepperMotor::Direction, std::string> {
					{StepperMotor::Direction::CCW, "<-CCW"},
					{StepperMotor::Direction::CW, "CW->"},
				};
				std::cout << dir_to_str_map.at(direction);
				std::cout.flush();
			}
		);
	};
	const auto delay_gtor = [](const std::size_t& timeout_ms) {
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
	};

	// WHEN
	StepperMotorManager<StepperCreateConfig> *instance_ptr(nullptr);
	StepperMotorResponse response;

	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = new StepperMotorManager<StepperCreateConfig>(motor_ctor, delay_gtor));
	ASSERT_NE(nullptr, instance_ptr);

	// run create
	ASSERT_NO_THROW(response = instance_ptr->run(test_create_request));
	ASSERT_EQ(response.code(), StepperMotorResponse::ResultCode::OK);

	// run steps
	ASSERT_NO_THROW(response = instance_ptr->run(test_steps_request));
	ASSERT_EQ(response.code(), StepperMotorResponse::ResultCode::OK);

	// run delete
	ASSERT_NO_THROW(response = instance_ptr->run(test_delete_request));
	ASSERT_EQ(response.code(), StepperMotorResponse::ResultCode::OK);


	// dtor
	ASSERT_NO_THROW(delete instance_ptr);
}