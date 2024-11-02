#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "circular_movement.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "test_stepper_motor.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;
using Axis = typename Vector<int>::Axis;

TEST(ut_circular_movement, ctor_dtor_sanity) {
	// GIVEN
	const CircularMovement::AxesAssignment test_assignment {
		{Axis::X, "test_motor_1"},
		{Axis::Y, "test_motor_2"},
		{Axis::Z, "test_motor_3"}
	};
	const unsigned int time_multiplier(1000);
	const auto delay_function = [](const CircularMovement::TimeUnit& time) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	InMemoryInventory<ResourceId, StepperMotor> inventory;

	// WHEN
	std::unique_ptr<CircularMovement> instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(
		instance_ptr = std::unique_ptr<CircularMovement>(
			new CircularMovement(
				&inventory,
				delay_function,
				test_assignment,
				time_multiplier	
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_circular_movement, perform_sanity) {
	// GIVEN
	const CircularMovement::AxesAssignment test_assignment {
		{Axis::X, "test_motor_1"},
		{Axis::Y, "test_motor_2"},
		{Axis::Z, "test_motor_3"}
	};
	const unsigned int time_multiplier(1000000); // us
	const auto delay_function = [](const CircularMovement::TimeUnit& time) {
		std::this_thread::sleep_for(std::chrono::microseconds(time));
	};
	Object test_vector;
	test_vector.add("x", Integer(500));
	test_vector.add("y", Integer(500));
	test_vector.add("z", Integer(0));

	Object test_rotation_vector;
	test_rotation_vector.add("x", Integer(500));
	test_rotation_vector.add("y", Integer(0));
	test_rotation_vector.add("z", Integer(0));
	const unsigned int test_feed(200);
	Object config;
	config.add("feed", Integer(static_cast<int>(test_feed)));
	config.add("target", test_vector);
	config.add("direction", Integer(static_cast<int>(CircularMovement::Direction::CW)));
	config.add("rotation_center", test_rotation_vector);

	// WHEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;
	auto step_action = [](const ResourceId& motor_id, const StepperMotor::Direction& dir) {
		std::cout << motor_id << " steps in " << std::to_string(static_cast<int>(dir)) << " direction" << std::endl;
	};
	for (const auto [axis, motor_id]: test_assignment) {
		auto action = [step_action, motor_id](const StepperMotor::Direction& dir) {
			step_action(motor_id, dir);
		};
		inventory.add(motor_id, new TestStepperMotor(action));
	}

	CircularMovement instance(
		&inventory,
		delay_function,
		test_assignment,
		time_multiplier
	);
	
	// THEN
	ASSERT_NO_THROW(instance.perform(config));
}