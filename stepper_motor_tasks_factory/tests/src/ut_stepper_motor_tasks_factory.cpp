#include "gtest/gtest.h"
#include <memory>
#include <stdexcept>
#include <string>

#include "array.hpp"
#include "custom_parser.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_tasks_factory.hpp"
#include "stepper_motor_tasks_factory_fixture.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;
using namespace mcu_factory;
using namespace mcu_factory_uts;

TEST_F(StepperMotorTasksFactoryFixture, ctor_dtor_sanity) {
	// GIVEN


	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(

			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new TestFactory(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}