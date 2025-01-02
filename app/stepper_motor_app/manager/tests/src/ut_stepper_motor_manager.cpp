#include <string>

#include "gtest/gtest.h"
#include "object.hpp"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_data.hpp"
#include "string.hpp"
#include "test_providers.hpp"

using namespace manager;

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// GIVEN
	auto test_manager_cfg = Object();
	test_manager_cfg.add("manager_id", String("test_manager"));
	host_tests::TestProviders<StepperMotorProviderType> providers;

	// WHEN
	StepperMotorManager *instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = new StepperMotorManager(&providers, test_manager_cfg));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(delete instance_ptr);
}