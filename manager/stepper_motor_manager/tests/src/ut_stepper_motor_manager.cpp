#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "array.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "server_types.hpp"
#include "string.hpp"
#include "test_gpo.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;
using GpoState = typename Gpo::State;

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, Gpio> gpio_inventory;
	InMemoryInventory<ResourceId, StepperMotor> stepper_motor_inventory;

	// WHEN
	using StepperMotorManagerUnqPtr = std::unique_ptr<StepperMotorManager>;
	StepperMotorManagerUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = StepperMotorManagerUnqPtr(new StepperMotorManager(&stepper_motor_inventory, &gpio_inventory)));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_stepper_motor_manager, crud_methods_sanity) {
	// GIVEN
	TestGpo ena, enb;
	TestGpo a_top, a_btm, b_top, b_btm;
	InMemoryInventory<ResourceId, Gpio> gpio_inventory;
	gpio_inventory.add("ena", &ena);
	gpio_inventory.add("enb", &enb);
	gpio_inventory.add("a_top", &a_top);
	gpio_inventory.add("a_btm", &a_btm);
	gpio_inventory.add("b_top", &b_top);
	gpio_inventory.add("b_btm", &b_btm);

	InMemoryInventory<ResourceId, StepperMotor> stepper_motor_inventory;

	const auto id(ResourceId("test_motor"));

	Object control_outputs;
	control_outputs.add("ena", String("ena"));
	control_outputs.add("enb", String("enb"));

	Object direction_outputs;
	direction_outputs.add("a_top", String("a_top"));
	direction_outputs.add("a_btm", String("a_btm"));
	direction_outputs.add("b_top", String("b_top"));
	direction_outputs.add("b_btm", String("b_btm"));

	Object state0;
	state0.add("a_top", Integer(static_cast<int>(GpoState::HIGH)));
	state0.add("a_btm", Integer(static_cast<int>(GpoState::LOW)));
	state0.add("b_top", Integer(static_cast<int>(GpoState::LOW)));
	state0.add("b_btm", Integer(static_cast<int>(GpoState::LOW)));
	Object state1;
	state1.add("a_top", Integer(static_cast<int>(GpoState::LOW)));
	state1.add("a_btm", Integer(static_cast<int>(GpoState::LOW)));
	state1.add("b_top", Integer(static_cast<int>(GpoState::HIGH)));
	state1.add("b_btm", Integer(static_cast<int>(GpoState::LOW)));
	Array states;
	states.push_back(state0);
	states.push_back(state1);

	Object create_cfg;
	create_cfg.add("control_outputs", control_outputs);
	create_cfg.add("direction_outputs", direction_outputs);
	create_cfg.add("states", states);

	Object update_enable_cfg;
	update_enable_cfg.add("type", String("update_enable_status"));
	update_enable_cfg.add("enable", Integer(1));
	Object update_steps_cfg;
	update_steps_cfg.add("type", String("steps"));
	update_steps_cfg.add("direction", Integer(static_cast<int>(StepperMotor::Direction::CCW)));
	update_steps_cfg.add("steps_number", Integer(10));

	// WHEN
	StepperMotorManager instance(
		&stepper_motor_inventory,
		&gpio_inventory
	);
	Object read_data;

	// THEN
	// create
	ASSERT_NO_THROW(instance.create_resource(id, create_cfg));
	ASSERT_TRUE(stepper_motor_inventory.contains(id));
	ASSERT_FALSE(stepper_motor_inventory.access(id).enabled());

	// read
	ASSERT_NO_THROW(read_data = instance.read_resource({id}));
	ASSERT_TRUE(read_data.contains("enabled"));
	ASSERT_FALSE(static_cast<bool>(Data::cast<Integer>(read_data.access("enabled")).get()));

	// update
	ASSERT_NO_THROW(instance.update_resource({id}, update_enable_cfg));
	ASSERT_TRUE(stepper_motor_inventory.access(id).enabled());
	
	// update
	ASSERT_NO_THROW(instance.update_resource({id}, update_steps_cfg));

	// delete
	ASSERT_NO_THROW(instance.delete_resource({id}));
	ASSERT_FALSE(stepper_motor_inventory.contains(id));
}