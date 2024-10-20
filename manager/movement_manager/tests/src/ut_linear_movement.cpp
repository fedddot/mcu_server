#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "stepper_motor.hpp"
#include "linear_movement.hpp"
#include "in_memory_inventory.hpp"
#include "server_types.hpp"

using namespace server;
using namespace manager;

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