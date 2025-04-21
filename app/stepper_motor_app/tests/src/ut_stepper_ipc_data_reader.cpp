#include "json/config.h"
#include "json/writer.h"
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/value.h"

#include "stepper_ipc_data_reader.hpp"
#include "stepper_motor_request.hpp"
#include "test_ipc_data_reader.hpp"

using namespace ipc;
using namespace manager;

using RawData = typename StepperIpcDataReader::RawData;

TEST(ut_stepper_ipc_data_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto test_raw_data_reader = TestIpcDataReader<RawData>(
		[]() -> std::optional<RawData> {
			return std::optional<RawData>();
		}
	);
	
	// WHEN
	StepperIpcDataReader *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new StepperIpcDataReader(test_raw_data_reader));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_stepper_ipc_data_reader, read_sanity) {
	// GIVEN
	const auto test_stepper_request = StepperMotorRequest {
		.direction = StepperMotor::Direction::CW,
		.steps_number = 100,
		.step_duration_ms = 10
	};
	auto json_stepper_request = Json::Value();
	switch (test_stepper_request.direction) {
		case StepperMotor::Direction::CW:
			json_stepper_request["direction"] = Json::String("CW");
			break;
		case StepperMotor::Direction::CCW:
			json_stepper_request["direction"] = Json::String("CCW");
			break;
		default:
			throw std::invalid_argument("invalid stepper motor direction");
	}
	json_stepper_request["steps_number"] = Json::UInt64(test_stepper_request.steps_number);
	json_stepper_request["step_duration_ms"] = Json::UInt64(test_stepper_request.step_duration_ms);

	const auto serial_json_stepper_request = Json::writeString(Json::StreamWriterBuilder(), json_stepper_request);

	const auto test_raw_data_reader = TestIpcDataReader<RawData>(
		[serial_json_stepper_request]() -> std::optional<RawData> {
			return std::optional<RawData>(RawData(serial_json_stepper_request.begin(), serial_json_stepper_request.end()));
		}
	);
	
	// WHEN
	StepperIpcDataReader instance(test_raw_data_reader);
	auto read_result = std::optional<StepperMotorRequest>();

	// THEN
	ASSERT_NO_THROW(read_result = instance.read());
	ASSERT_TRUE(read_result);
	ASSERT_EQ(test_stepper_request, *read_result);
}

// TEST(ut_stepper_ipc_data_reader, read_missing_raw_data_negative) {
// 	// WHEN
// 	const auto raw_data_reader = TestRawReader(std::optional<RawData>());
// 	auto instance = JsonIpcDataReader<TestIpcData>(raw_data_reader, parse_ipc_data);
// 	auto read_result = std::optional<TestIpcData>();

// 	// THEN
// 	ASSERT_NO_THROW(read_result = instance.read());
// 	ASSERT_FALSE(read_result);
// }

// inline RawData serialize_ipc_data(const TestIpcData& ipc_data) {
// 	const auto stepper_val = Json::String(ipc_data);
// 	const auto writer_builder = Json::StreamWriterBuilder();
//     const auto serial_str = Json::writeString(writer_builder, stepper_val);
//     return RawData(serial_str.begin(), serial_str.end());
// }

// inline TestIpcData parse_ipc_data(const Json::Value& stepper_data) {
// 	if (!stepper_data.isString()) {
// 		throw std::invalid_argument("stepper data is expected to be a string");
// 	}
// 	return TestIpcData(stepper_data.asString());
// }