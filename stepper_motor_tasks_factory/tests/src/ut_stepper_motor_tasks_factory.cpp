#include "gtest/gtest.h"
#include <string>

#include "stepper_motor_tasks_factory.hpp"

using namespace mcu_server;
using namespace mcu_factory;
using namespace mcu_factory_uts;
using namespace mcu_platform;



TEST(ut_stepper_motor_tasks_factory, ctor_dtor_sanity) {
	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(
				platform(),
				parsers(),
				result_reporter(),
				result_state_reporter(),
				tasks_results_reporter()
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