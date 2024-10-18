#include <memory>

#include "gtest/gtest.h"

#include "array.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor.hpp"
#include "movement_manager.hpp"
#include "in_memory_inventory.hpp"
#include "server_types.hpp"
#include "string.hpp"
#include "test_stepper_motor.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// GIVEN
	InMemoryInventory<ResourceId, StepperMotor> inventory;

	// WHEN
	using MovementManagerUnqPtr = std::unique_ptr<MovementManager>;
	MovementManagerUnqPtr instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = MovementManagerUnqPtr(new MovementManager(&inventory)));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_movement_manager, crud_methods_sanity) {
	// GIVEN
	const ResourceId movement_id("movement_id");
	const ResourceId motor_id_1("motor_1");
	Object steps_cfg1;
	steps_cfg1.add("dir", Integer(static_cast<int>(StepperMotor::Direction::CW)));
	steps_cfg1.add("steps_num", Integer(100));
	steps_cfg1.add("on_time", Integer(500));
	steps_cfg1.add("off_time", Integer(100));
	Object movement_cfg_1;
	movement_cfg_1.add("motor_id", String(motor_id_1));
	movement_cfg_1.add("steps_cfg", steps_cfg1);

	const ResourceId motor_id_2("motor_2");
	Object steps_cfg2;
	steps_cfg2.add("dir", Integer(static_cast<int>(StepperMotor::Direction::CCW)));
	steps_cfg2.add("steps_num", Integer(1000));
	steps_cfg2.add("on_time", Integer(5000));
	steps_cfg2.add("off_time", Integer(1000));
	Object movement_cfg_2;
	movement_cfg_2.add("motor_id", String(motor_id_2));
	movement_cfg_2.add("steps_cfg", steps_cfg2);

	Array movements;
	movements.push_back(movement_cfg_1);
	movements.push_back(movement_cfg_2);

	Object config;
	config.add("movements", movements);
	config.add("repeats", Integer(100));

	Body create_request_body;
	create_request_body.add("id", String(movement_id));
	create_request_body.add("config", config);

	// WHEN
	auto assertion_callback = [](const Object& expected_steps_cfg, const StepperMotor::Direction& direction, const unsigned int steps_num, const unsigned int on_time, const unsigned int off_time) {
		ASSERT_EQ(static_cast<StepperMotor::Direction>(Data::cast<Integer>(expected_steps_cfg.access("dir")).get()), direction);
		ASSERT_EQ(static_cast<unsigned int>(Data::cast<Integer>(expected_steps_cfg.access("steps_num")).get()), steps_num);
		ASSERT_EQ(static_cast<unsigned int>(Data::cast<Integer>(expected_steps_cfg.access("on_time")).get()), on_time);
		ASSERT_EQ(static_cast<unsigned int>(Data::cast<Integer>(expected_steps_cfg.access("off_time")).get()), off_time);
	};

	InMemoryInventory<ResourceId, StepperMotor> inventory;
	inventory.add(
		motor_id_1,
		new TestStepperMotor(
			[steps_cfg1, assertion_callback](const StepperMotor::Direction& direction, const unsigned int steps_num, const unsigned int on_time, const unsigned int off_time) {
				assertion_callback(steps_cfg1, direction, steps_num, on_time, off_time);
			}
		)
	);
	inventory.add(
		motor_id_2,
		new TestStepperMotor(
			[steps_cfg2, assertion_callback](const StepperMotor::Direction& direction, const unsigned int steps_num, const unsigned int on_time, const unsigned int off_time) {
				assertion_callback(steps_cfg2, direction, steps_num, on_time, off_time);
			}
		)
	);
	MovementManager instance(&inventory);

	// THEN
	// create
	ASSERT_NO_THROW(instance.create_resource(create_request_body));
}