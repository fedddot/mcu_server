#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "data.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "request.hpp"
#include "response.hpp"

using namespace server;
using namespace manager;
// using namespace manager_uts;

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

// static inline StepperMotor *test_stepper_motor_ctor(const Data& data) {
// 	auto dir = static_cast<StepperMotor::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("dir")).get());
// 	switch (dir) {
// 	case StepperMotor::Direction::IN:
// 		return new TestGpi();
// 	case StepperMotor::Direction::OUT:
// 		return new TestGpo();
// 	default:
// 		throw std::invalid_argument("unsupported StepperMotor direction received");
// 	}
// }

// TEST(ut_stepper_motor_manager, run_request_sanity) {
// 	// GIVEN
// 	Body create_gpo_data;
// 	create_gpo_data.add("id", String("gpo_id"));
// 	create_gpo_data.add("dir", Integer(static_cast<int>(StepperMotor::Direction::OUT)));

// 	Body update_gpo_data;
// 	update_gpo_data.add("state", Integer(static_cast<int>(StepperMotor::State::HIGH)));

// 	Body create_gpi_data;
// 	create_gpi_data.add("id", String("gpi_id"));
// 	create_gpi_data.add("dir", Integer(static_cast<int>(StepperMotor::Direction::IN)));
	
// 	// WHEN
// 	StepperMotorManager instance(test_stepper_motor_ctor);
// 	Response response(ResponseCode::OK, Body());

// 	// THEN
// 	// Create GPO
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::CREATE, 
// 				{}, 
// 				create_gpo_data
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());

// 	// Create GPI
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::CREATE, 
// 				{}, 
// 				create_gpi_data
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());

// 	// ReadAll
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::READ, 
// 				{}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// 	ASSERT_TRUE(response.body().contains("members"));
// 	ASSERT_EQ(Data::Type::ARRAY, response.body().access("members").type());
// 	ASSERT_EQ(2UL, Data::cast<Array>(response.body().access("members")).size());

// 	// Update
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::UPDATE, 
// 				{Data::cast<String>(create_gpo_data.access("id")).get()}, 
// 				update_gpo_data
// 			)
// 		)
// 	);
	
// 	// Read GPI
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::READ, 
// 				{Data::cast<String>(create_gpi_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// 	ASSERT_TRUE(response.body().contains("id"));
// 	ASSERT_EQ(Data::cast<String>(create_gpi_data.access("id")).get(), Data::cast<String>(response.body().access("id")).get());
// 	ASSERT_TRUE(response.body().contains("state"));
// 	ASSERT_EQ(StepperMotor::State::LOW, static_cast<StepperMotor::State>(Data::cast<Integer>(response.body().access("state")).get()));

// 	// Read GPO
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::READ, 
// 				{Data::cast<String>(create_gpo_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// 	ASSERT_TRUE(response.body().contains("id"));
// 	ASSERT_EQ(Data::cast<String>(create_gpo_data.access("id")).get(), Data::cast<String>(response.body().access("id")).get());
// 	ASSERT_TRUE(response.body().contains("state"));
// 	ASSERT_EQ(StepperMotor::State::HIGH, static_cast<StepperMotor::State>(Data::cast<Integer>(response.body().access("state")).get()));

// 	// Delete GPI
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::DELETE, 
// 				{Data::cast<String>(create_gpi_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());

// 	// Delete GPO
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::DELETE, 
// 				{Data::cast<String>(create_gpo_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// }