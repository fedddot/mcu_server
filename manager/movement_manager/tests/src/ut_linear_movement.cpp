#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "linear_movement.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "test_stepper_motor.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;

TEST(ut_linear_movement, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;

	// WHEN
	using LinearMovementUnqPtr = std::unique_ptr<LinearMovement>;
	LinearMovementUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(
		instance_ptr = LinearMovementUnqPtr(
			new LinearMovement(
				&inventory,
				[](const unsigned int) {throw std::runtime_error("NOT IMPLEMENTED");},
				false
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_linear_movement, perform_sanity) {
	// GIVEN
	Object steps1;
	steps1.add("motor_x", Integer(-120));
	steps1.add("motor_y", Integer(13));
	steps1.add("motor_z", Integer(14));
	Object steps2;
	steps2.add("motor_x", Integer(+120));
	steps2.add("motor_y", Integer(-13));
	steps2.add("motor_z", Integer(-14));
	Array steps;
	steps.push_back(steps1);
	steps.push_back(steps2);

	Object config;
	config.add("step_duration", Integer(10));
	config.add("steps", steps);

	const bool inverse_direction(false);
	auto delay = [](const unsigned int delay) {
		std::cout << "delay: " << std::to_string(delay) << std::endl;
	};

	// WHEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;
	Data::cast<Object>(steps.access(0)).for_each(
		[&inventory](const std::string& motor_id, const Data&) {
			inventory.add(
				motor_id,
				new TestStepperMotor(
					[motor_id](const StepperMotor::Direction& direction) {
						std::cout << motor_id << " steps in direction " << std::to_string(static_cast<int>(direction)) << std::endl;
					}
				)
			);
		}
	);
	LinearMovement instance(
		&inventory,
		delay,
		inverse_direction
	);
	
	// THEN
	ASSERT_NO_THROW(instance.perform(config));
}