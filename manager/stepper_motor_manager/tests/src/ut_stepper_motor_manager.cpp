#include <iostream>
#include <memory>

#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "test_stepper_motor.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;

static StepperMotor *create_test_stepper_motor(const TestStepperMotor::StepAction& action, const Data& create_cfg) {
	(void)create_cfg;
	return new TestStepperMotor(action);
}

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;
	auto create_action = [](const Data& create_cfg)-> StepperMotor * {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	using StepperMotorManagerUnqPtr = std::unique_ptr<StepperMotorManager>;
	StepperMotorManagerUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = StepperMotorManagerUnqPtr(new StepperMotorManager(&inventory, create_action)));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_stepper_motor_manager, crud_methods_sanity) {
	// GIVEN
	const auto id(ResourceId("test_motor"));
	auto steps_action_called(false);
	auto step_action = [&steps_action_called](const StepperMotor::Direction& dir) {
		std::cout << "a step in direction " << std::to_string(static_cast<int>(dir)) << std::endl;
		steps_action_called = true;
	};
	Object create_cfg;
	
	Object update_enable_cfg;
	update_enable_cfg.add("type", String("update_enable_status"));
	update_enable_cfg.add("enable", Integer(1));
	Object update_steps_cfg;
	update_steps_cfg.add("type", String("steps"));
	update_steps_cfg.add("direction", Integer(static_cast<int>(StepperMotor::Direction::CCW)));
	update_steps_cfg.add("steps_number", Integer(10));

	// WHEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;
	StepperMotorManager instance(
		&inventory,
		[step_action](const Data& create_cfg) {
			return create_test_stepper_motor(step_action, create_cfg);
		}
	);
	Object read_data;

	// THEN
	// create
	ASSERT_NO_THROW(instance.create_resource(id, create_cfg));
	ASSERT_TRUE(inventory.contains(id));
	ASSERT_FALSE(inventory.access(id).enabled());

	// read
	ASSERT_NO_THROW(read_data = instance.read_resource({id}));
	ASSERT_TRUE(read_data.contains("enabled"));
	ASSERT_FALSE(static_cast<bool>(Data::cast<Integer>(read_data.access("enabled")).get()));

	// update
	ASSERT_NO_THROW(instance.update_resource({id}, update_enable_cfg));
	ASSERT_TRUE(inventory.access(id).enabled());
	
	// update
	ASSERT_NO_THROW(instance.update_resource({id}, update_steps_cfg));
	ASSERT_TRUE(steps_action_called);

	// delete
	ASSERT_NO_THROW(instance.delete_resource({id}));
	ASSERT_FALSE(inventory.contains(id));
}