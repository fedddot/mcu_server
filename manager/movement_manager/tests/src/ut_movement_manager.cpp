#include <cstddef>

#include "gtest/gtest.h"

#include "movement_manager.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"

using namespace manager;

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// WHEN
	MovementManager *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance = new MovementManager(
			[]() -> MovementManager::Steppers {
				return MovementManager::Steppers {
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

TEST(ut_movement_manager, run_sanity) {
	// GIVEN
	const auto test_request = StepperMotorRequest {
		.motor_id = "test_motor_id",
		.direction = Direction::CCW,
		.steps_number = 100,
		.step_duration_ms = 10
	};

	// WHEN
	MovementManager instance(
		[test_request]() -> MovementManager::Steppers {
			return MovementManager::Steppers {
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