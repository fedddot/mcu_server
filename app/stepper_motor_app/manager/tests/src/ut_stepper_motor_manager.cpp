#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "stepper_motor_manager.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"
#include "test_providers.hpp"
#include "test_stepper_motor_creator.hpp"

using namespace manager;
using namespace manager_tests;

using StepperCreateConfig = std::string;
using TestStepperMotorRequest = StepperMotorRequest<StepperCreateConfig>;

TEST(ut_stepper_motor_manager, sanity) {
	// GIVEN
	auto test_manager_cfg = StepperMotorManagerConfig("stepper motor manager");
	
	const auto test_motor_id = StepperMotorId("test_motor");
	const auto test_motor_create_cfg = StepperCreateConfig("test_motor_create_cfg");
	auto test_create_request = TestStepperMotorRequest(TestStepperMotorRequest::Type::CREATE_STEPPER, test_motor_id);
	test_create_request.set_create_config(test_motor_create_cfg);

	auto test_delete_request = TestStepperMotorRequest(TestStepperMotorRequest::Type::DELETE_STEPPER, test_motor_id);

	host_tests::TestProviders<StepperMotorProviderType> providers;

	// WHEN
	providers.add_provider(
		StepperMotorProviderType::MOTOR_CREATOR,
		new TestStepperMotorCreator<StepperCreateConfig>(
			[](const StepperMotorDirection& direction) {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);
	StepperMotorManager<StepperCreateConfig> *instance_ptr(nullptr);
	StepperMotorResponse response;

	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = new StepperMotorManager<StepperCreateConfig>(&providers));
	ASSERT_NE(nullptr, instance_ptr);

	// run create
	ASSERT_NO_THROW(response = instance_ptr->run(test_create_request));
	ASSERT_EQ(response.code(), StepperMotorResponse::ResultCode::OK);

	// run delete
	ASSERT_NO_THROW(response = instance_ptr->run(test_delete_request));
	ASSERT_EQ(response.code(), StepperMotorResponse::ResultCode::OK);


	// dtor
	ASSERT_NO_THROW(delete instance_ptr);
}