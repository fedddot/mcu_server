#include <memory>
#include <stdexcept>

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

static StepperMotor *create_test_stepper_motor(const Body& request_body) {
	throw std::runtime_error("NOT_IMPLEMENTED");
}

static Body read_stepper_motor(const StepperMotor& motor) {
	return Body();
}

static void write_stepper_motor(StepperMotor *motor, const Data& config) {
	
}

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;

	// WHEN
	using StepperMotorManagerUnqPtr = std::unique_ptr<StepperMotorManager>;
	StepperMotorManagerUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = StepperMotorManagerUnqPtr(new StepperMotorManager(&inventory, create_test_stepper_motor, read_stepper_motor, write_stepper_motor)));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_stepper_motor_manager, crud_methods_sanity) {
	// GIVEN
	const auto expected_direction(StepperMotor::Direction::CCW);
	const ResourceId id("test_motor");

	Body create_body;
	create_body.add("id", String(id));
	create_body.add("config", Object());

	Object update_cfg;
	update_cfg.add("dir", Integer(static_cast<int>(expected_direction)));
	Body update_body;
	update_body.add("config", update_cfg);

	auto steps_action = [expected_direction](const StepperMotor::Direction& direction) {
		ASSERT_EQ(expected_direction, direction);
	};
	auto creator = [steps_action](const Body& request_body) {
		(void)request_body;
		return new manager_uts::TestStepperMotor(steps_action);
	};

	// WHEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;
	StepperMotorManager instance(&inventory, creator, read_stepper_motor, write_stepper_motor);
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