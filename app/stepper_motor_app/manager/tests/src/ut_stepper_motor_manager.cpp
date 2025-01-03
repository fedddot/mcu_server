#include <string>

#include "gtest/gtest.h"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_types.hpp"
#include "test_providers.hpp"

using namespace manager;

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// GIVEN
	using StepperCreateConfig = std::string;
	auto test_manager_cfg = StepperMotorManagerConfig("stepper motor manager");
	host_tests::TestProviders<StepperMotorProviderType> providers;
	auto test_create_request = CreateStepperMotorRequest<StepperCreateConfig>("test_motor", StepperCreateConfig("test_cfg"));

	// WHEN
	StepperMotorManager<StepperCreateConfig> *instance_ptr(nullptr);
	StepperMotorResponse response;

	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = new StepperMotorManager<StepperCreateConfig>(&providers, test_manager_cfg));
	ASSERT_NE(nullptr, instance_ptr);

	// run
	ASSERT_NO_THROW(response = instance_ptr->run(test_create_request));
	ASSERT_EQ(response.code(), StepperMotorResponseCode::OK);

	// dtor
	ASSERT_NO_THROW(delete instance_ptr);
}