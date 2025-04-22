#include <stdexcept>
#include <string>

#include "json/reader.h"
#include "json/value.h"

#include "gtest/gtest.h"

#include "stepper_ipc_data_infra.hpp"
#include "stepper_ipc_data_writer.hpp"
#include "stepper_motor_response.hpp"
#include "test_ipc_data_writer.hpp"

using namespace ipc;
using namespace manager;

TEST(ut_stepper_ipc_data_writer, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_writer = TestIpcDataWriter<RawData>(
		[](const RawData&) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);

	// WHEN
	StepperIpcDataWriter *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new StepperIpcDataWriter(raw_data_writer));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_stepper_ipc_data_writer, write_sanity) {
	// GIVEN
	auto test_response = StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
	test_response.set_state(StepperMotor::State::ENABLED);
	test_response.set_message("test message");

	const auto raw_data_writer = TestIpcDataWriter<RawData>(
		[test_response](const RawData& raw_data) {
			Json::Value root;
	    	Json::Reader reader;
			ASSERT_TRUE(reader.parse(std::string(raw_data.begin(), raw_data.end()), std::ref(root), true));
			const auto received_response = json_value_to_stepper_response(root);
			ASSERT_EQ(test_response.code(), received_response.code());
			ASSERT_TRUE(received_response.message().has_value());
			ASSERT_EQ(test_response.message().value(), received_response.message().value());
			ASSERT_TRUE(received_response.state().has_value());
			ASSERT_EQ(test_response.state().value(), received_response.state().value());
		}
	);
	
	// WHEN
	auto instance = StepperIpcDataWriter(raw_data_writer);
	
	// THEN
	ASSERT_NO_THROW(instance.write(test_response));
}