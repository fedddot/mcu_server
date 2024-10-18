#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "test_stepper_motor.hpp"

using namespace server;
using namespace manager;

static StepperMotor *create_test_stepper_motor(const Body& request_body) {
	throw std::runtime_error("NOT_IMPLEMENTED");
}

static Body read_stepper_motor(const StepperMotor& motor) {
	return Body();
}

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;

	// WHEN
	using StepperMotorManagerUnqPtr = std::unique_ptr<StepperMotorManager>;
	StepperMotorManagerUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = StepperMotorManagerUnqPtr(new StepperMotorManager(&inventory, create_test_stepper_motor, read_stepper_motor)));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_stepper_motor_manager, crud_methods_sanity) {
	// GIVEN
	const auto expected_direction(StepperMotor::Direction::CCW);
	const unsigned int expected_steps_num(120);
	const unsigned int expected_on_time(20);
	const unsigned int expected_off_time(30);
	const ResourceId id("test_motor");

	Body create_body;
	create_body.add("id", String(id));
	create_body.add("config", Object());

	Object update_cfg;
	update_cfg.add("dir", Integer(static_cast<int>(expected_direction)));
	update_cfg.add("steps_num", Integer(expected_steps_num));
	update_cfg.add("on_time", Integer(expected_on_time));
	update_cfg.add("off_time", Integer(expected_off_time));
	Body update_body;
	update_body.add("config", update_cfg);

	auto steps_action = [expected_direction, expected_steps_num, expected_on_time, expected_off_time](const StepperMotor::Direction& direction, const unsigned int steps_num, const unsigned int on_time, const unsigned int off_time) {
		ASSERT_EQ(expected_direction, direction);
		ASSERT_EQ(expected_steps_num, steps_num);
		ASSERT_EQ(expected_on_time, on_time);
		ASSERT_EQ(expected_off_time, off_time);
	};
	auto creator = [steps_action](const Body& request_body) {
		(void)request_body;
		return new manager_uts::TestStepperMotor(steps_action);
	};

	// WHEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;
	StepperMotorManager instance(&inventory, creator, read_stepper_motor);
	Body response_body;

	// THEN
	// create
	ASSERT_NO_THROW(instance.create_resource(create_body));
	ASSERT_TRUE(inventory.contains(id));

	// read
	ASSERT_NO_THROW(response_body = instance.read_resource({id}));

	// update
	ASSERT_NO_THROW(instance.update_resource({id}, update_body));

	// delete
	ASSERT_NO_THROW(instance.delete_resource({id}));
	ASSERT_FALSE(inventory.contains(id));
}