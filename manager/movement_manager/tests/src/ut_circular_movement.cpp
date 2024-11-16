#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "double.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "circular_movement.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;
using Axis = typename Vector<double>::Axis;

TEST(ut_circular_movement, ctor_dtor_sanity) {
	// GIVEN
	const CircularMovement::AxesAssignment test_assignment {
		{Axis::X, "test_motor_1"},
		{Axis::Y, "test_motor_2"},
		{Axis::Z, "test_motor_3"}
	};
	const auto delay_function = [](const double& time) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const unsigned int steps_per_length(100);
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
				steps_per_length
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
	const auto delay_function = [](const double& time) {
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned int>(time) * 1000));
	};
	Object test_vector;
	test_vector.add("x", Double(45));
	test_vector.add("y", Double(45));
	test_vector.add("z", Double(0));

	Object test_rotation_vector;
	test_rotation_vector.add("x", Double(45));
	test_rotation_vector.add("y", Double(0));
	test_rotation_vector.add("z", Double(0));
	const unsigned int test_feed(3);
	const unsigned int steps_per_length(100);
	Object config;
	config.add("feed", Double(static_cast<int>(test_feed)));
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
		steps_per_length
	);
	
	// THEN
	ASSERT_NO_THROW(instance.perform(config));
}