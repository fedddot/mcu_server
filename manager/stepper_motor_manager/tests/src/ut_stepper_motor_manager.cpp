#include <memory>

#include "gtest/gtest.h"
#include <stdexcept>

#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"

using namespace server;
using namespace manager;

static StepperMotor *create_test_stepper_motor(const Body& request_body) {
	throw std::runtime_error("NOT_IMPLEMENTED");
}

static Body read_stepper_motor(const StepperMotor& motor) {
	throw std::runtime_error("NOT_IMPLEMENTED");
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

// TEST(ut_stepper_motor_manager, crud_methods_sanity) {
// 	// GIVEN
// 	const auto dir(StepperMotor::Direction::OUT);
// 	const auto id(ResourceId("test_gpo"));
// 	const auto state(StepperMotor::State::HIGH);

// 	Object create_cfg;
// 	create_cfg.add("dir", Integer(static_cast<int>(dir)));
	
// 	Body create_body;
// 	create_body.add("id", String(id));
// 	create_body.add("config", create_cfg);

// 	Object update_cfg;
// 	update_cfg.add("state", Integer(static_cast<int>(state)));
// 	Body update_body;
// 	update_body.add("config", update_cfg);

// 	// WHEN
// 	InMemoryInventory<ResourceId, StepperMotor> inventory;
// 	StepperMotorManager instance(&inventory, create_test_stepper_motor);
// 	Body response_body;

// 	// THEN
// 	// create
// 	ASSERT_NO_THROW(instance.create_resource(create_body));
// 	ASSERT_TRUE(inventory.contains(id));
// 	ASSERT_EQ(dir, inventory.access(id).direction());

// 	// read
// 	ASSERT_NO_THROW(response_body = instance.read_resource({id}));
// 	ASSERT_TRUE(response_body.contains("dir"));
// 	ASSERT_EQ(dir, static_cast<StepperMotor::Direction>(Data::cast<Integer>(response_body.access("dir")).get()));
// 	ASSERT_EQ(StepperMotor::State::LOW, static_cast<StepperMotor::State>(Data::cast<Integer>(response_body.access("state")).get()));

// 	// update
// 	ASSERT_NO_THROW(instance.update_resource({id}, update_body));
// 	ASSERT_EQ(state, StepperMotor::cast<Gpo>(inventory.access(id)).state());	

// 	// delete
// 	ASSERT_NO_THROW(instance.delete_resource({id}));
// 	ASSERT_FALSE(inventory.contains(id));
// }