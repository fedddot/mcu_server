#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "data.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "request.hpp"
#include "response.hpp"
#include "test_stepper_motor.hpp"

using namespace server;
using namespace manager;
using namespace manager_uts;

using Method = typename Request::Method;
using Path = typename Request::Path;
using Body = typename Request::Body;
using ResponseCode = typename Response::ResponseCode;	

TEST(ut_stepper_motor_manager, ctor_cctor_clone_dtor_id_sanity) {
	// GIVEN
	auto test_creator = [](const Data& data)-> StepperMotor * {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	using StepperMotorManagerUnqPtr = std::unique_ptr<StepperMotorManager>;
	using ResourceUnqPtr = std::unique_ptr<Resource>;
	StepperMotorManagerUnqPtr instance_ptr(nullptr);
	StepperMotorManagerUnqPtr instance_ptr_copy(nullptr);
	ResourceUnqPtr instance_ptr_clone(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = StepperMotorManagerUnqPtr(new StepperMotorManager(test_creator)));
	ASSERT_NE(nullptr, instance_ptr);

	// cctor
	ASSERT_NO_THROW(instance_ptr_copy = StepperMotorManagerUnqPtr(new StepperMotorManager(*instance_ptr)));
	ASSERT_NE(nullptr, instance_ptr_copy);

	// clone
	ASSERT_NO_THROW(instance_ptr_clone = ResourceUnqPtr(instance_ptr->clone()));
	ASSERT_NE(nullptr, instance_ptr_clone);

	// dtors
	ASSERT_NO_THROW(instance_ptr = nullptr);
	ASSERT_NO_THROW(instance_ptr_copy = nullptr);
	ASSERT_NO_THROW(instance_ptr_clone = nullptr);
}

static inline StepperMotor *test_stepper_motor_ctor(const Data& data, const TestStepperMotor::StepsAction& action) {
	(void)data;
	return new TestStepperMotor(action);
}

TEST(ut_stepper_motor_manager, run_request_sanity) {
	// GIVEN
	Body update_motor_data;
	update_motor_data.add("dir", Integer(static_cast<int>(StepperMotor::Direction::CW)));
	update_motor_data.add("steps_num", Integer(100));
	update_motor_data.add("step_duration_ms", Integer(10));

	Body create_motor_data;
	create_motor_data.add("id", String("test_stepper_motor"));

	auto steps_action = [](const StepperMotor::Direction&, unsigned int, unsigned int) {
		;
	};
	
	// WHEN
	StepperMotorManager instance(
		[steps_action](const Data& data) {
			return test_stepper_motor_ctor(data, steps_action);
		}
	);
	Response response(ResponseCode::OK, Body());

	// THEN
	// Create
	ASSERT_NO_THROW(
		response = instance.run_request(
			Request(
				Method::CREATE, 
				{}, 
				create_motor_data
			)
		)
	);
	ASSERT_EQ(ResponseCode::OK, response.code());

	// ReadAll
	ASSERT_NO_THROW(
		response = instance.run_request(
			Request(
				Method::READ, 
				{}, 
				Body()
			)
		)
	);
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("members"));
	ASSERT_EQ(Data::Type::ARRAY, response.body().access("members").type());
	ASSERT_EQ(1UL, Data::cast<Array>(response.body().access("members")).size());

	// Update
	ASSERT_NO_THROW(
		response = instance.run_request(
			Request(
				Method::UPDATE, 
				{Data::cast<String>(create_motor_data.access("id")).get()}, 
				update_motor_data
			)
		)
	);
	
	// Read
	ASSERT_NO_THROW(
		response = instance.run_request(
			Request(
				Method::READ, 
				{Data::cast<String>(create_motor_data.access("id")).get()}, 
				Body()
			)
		)
	);
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("id"));
	ASSERT_EQ(Data::cast<String>(create_motor_data.access("id")).get(), Data::cast<String>(response.body().access("id")).get());

	// Delete
	ASSERT_NO_THROW(
		response = instance.run_request(
			Request(
				Method::DELETE, 
				{Data::cast<String>(create_motor_data.access("id")).get()}, 
				Body()
			)
		)
	);
	ASSERT_EQ(ResponseCode::OK, response.code());
}