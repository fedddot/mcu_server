#include <cstddef>

#include "gtest/gtest.h"

#include "stepper_motor_data.hpp"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "test_stepper_motor.hpp"

using namespace manager;
using namespace manager_tests;

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// WHEN
	StepperMotorManager *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance = new StepperMotorManager(
			[]() -> StepperMotorManager::Steppers {
				return StepperMotorManager::Steppers {
					{
						"motor_1",
						std::make_shared<TestStepperMotor>(
							[](const Direction&) {
								throw std::runtime_error("NOT IMPLEMENTED");
							}
						)
					},
				};
			},
			[](const std::size_t& timeout_ms) {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}

TEST(ut_stepper_motor_manager, run_sanity) {
	// GIVEN
	const auto test_request = StepperMotorRequest {
		.motor_id = "test_motor_id",
		.direction = Direction::CCW,
		.steps_number = 100,
		.step_duration_ms = 10
	};

	// WHEN
	StepperMotorManager instance(
		[test_request]() -> StepperMotorManager::Steppers {
			return StepperMotorManager::Steppers {
				{
					test_request.motor_id,
					std::make_shared<TestStepperMotor>(
						[test_request](const Direction& direction) {
							ASSERT_EQ(test_request.direction, direction);
						}
					)
				},
			};
		},
		[test_request](const std::size_t& timeout_ms) {
			ASSERT_EQ(test_request.step_duration_ms, timeout_ms);
		}
	);
	StepperMotorResponse response;

	// THEN
	ASSERT_NO_THROW(response = instance.run(test_request));
}