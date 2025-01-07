#include <string>

#include "gtest/gtest.h"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_request_data.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"
#include "test_providers.hpp"

using namespace manager;

TEST(ut_stepper_motor_manager, ctor_dtor_sanity) {
	// GIVEN
	using StepperCreateConfig = std::string;
	auto test_manager_cfg = StepperMotorManagerConfig("stepper motor manager");
	host_tests::TestProviders<StepperMotorProviderType> providers;
	const auto test_create_request_data = StepperMotorCreateRequestData<StepperMotorId, StepperCreateConfig>("test_motor", StepperCreateConfig("test_cfg"));
	auto test_create_request = StepperMotorRequest(StepperMotorRequest::Type::CREATE_STEPPER);
	test_create_request.set_data(test_create_request_data);

	// WHEN
	StepperMotorManager<StepperCreateConfig> *instance_ptr(nullptr);
	StepperMotorResponse response;

	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = new StepperMotorManager<StepperCreateConfig>(&providers));
	ASSERT_NE(nullptr, instance_ptr);

	// run
	ASSERT_NO_THROW(response = instance_ptr->run(test_create_request));
	std::cout << "message: " << response.message() << std::endl;
	ASSERT_EQ(response.code(), StepperMotorResponse::ResultCode::OK);

	// dtor
	ASSERT_NO_THROW(delete instance_ptr);
}