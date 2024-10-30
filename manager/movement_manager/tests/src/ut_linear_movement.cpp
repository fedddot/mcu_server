#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

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
using Axis = typename Vector<int>::Axis;

TEST(ut_linear_movement, ctor_dtor_sanity) {
	// GIVEN
	const LinearMovement::AxesAssignment test_assignment {
		{Axis::X, "test_motor_1"},
		{Axis::Y, "test_motor_2"},
		{Axis::Z, "test_motor_3"}
	};
	const unsigned int time_multiplier(1000);
	const auto delay_function = [](const LinearMovement::TimeUnit& time) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	InMemoryInventory<ResourceId, StepperMotor> inventory;

	// WHEN
	std::unique_ptr<LinearMovement> instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(
		instance_ptr = std::unique_ptr<LinearMovement>(
			new LinearMovement(
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

TEST(ut_linear_movement, perform_sanity) {
	// GIVEN
	const LinearMovement::AxesAssignment test_assignment {
		{Axis::X, "test_motor_1"},
		{Axis::Y, "test_motor_2"},
		{Axis::Z, "test_motor_3"}
	};
	const unsigned int time_multiplier(1000); // ms
	const auto delay_function = [](const LinearMovement::TimeUnit& time) {
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
	};

	Object test_vector;
	test_vector.add("x", Integer(-12));
	test_vector.add("y", Integer(5));
	test_vector.add("z", Integer(-3));

	const unsigned int test_feed(500);
	Object config;
	config.add("feed", Integer(static_cast<int>(test_feed)));
	config.add("vector", test_vector);

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

	LinearMovement instance(
		&inventory,
		delay_function,
		test_assignment,
		time_multiplier
	);
	
	// THEN
	ASSERT_NO_THROW(instance.perform(config));
}