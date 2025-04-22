#include <cstddef>
#include <stdexcept>

#include "gtest/gtest.h"
#include "json/value.h"

#include "stepper_host.hpp"
#include "stepper_ipc_data_infra.hpp"
#include "stepper_motor_data.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"
#include "test_stepper_motor.hpp"

using namespace ipc;
using namespace manager;
using namespace host;

TEST(ut_stepper_host, ctor_dtor_sanity) {
	// GIVEN
	const auto raw_data_writer = TestIpcDataWriter<RawData>(
		[](const RawData& raw_data) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	const auto raw_data_reader = TestIpcDataReader<RawData>(
		[]() -> std::optional<RawData> {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	auto stepper_motor_creator = []() -> StepperMotor * {
		return new manager_tests::TestStepperMotor(
			[](const Direction&) {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		);
	};
	auto delay_generator = [](const std::size_t&) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	StepperHost *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new StepperHost(raw_data_reader, raw_data_writer, stepper_motor_creator, delay_generator));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_stepper_host, run_sanity) {
	// GIVEN
	const auto test_request = StepperMotorRequest {
		Direction::CCW,
		100,
		10
	};
	const auto expected_response = StepperMotorResponse {
		StepperMotorResponse::ResultCode::OK,
		std::nullopt,
		std::nullopt
	};
	const auto raw_data_writer = TestIpcDataWriter<RawData>(
		[expected_response](const RawData& raw_data) {
			auto json_response = Json::Value();
			auto reader = Json::Reader();
			ASSERT_TRUE(reader.parse(std::string(raw_data.begin(), raw_data.end()), std::ref(json_response)));
			const auto response = json_value_to_stepper_response(json_response);
			ASSERT_EQ(expected_response, response);
		}
	);
	const auto raw_data_reader = TestIpcDataReader<RawData>(
		[test_request]() -> std::optional<RawData> {
			const auto json_request = stepper_request_to_json_value(test_request);
			const auto serial_request = Json::FastWriter().write(json_request);
			return RawData(serial_request.begin(), serial_request.end());
		}
	);
	auto stepper_motor_creator = [test_request]() -> StepperMotor * {
		return new manager_tests::TestStepperMotor(
			[test_request](const Direction& direction) {
				ASSERT_EQ(test_request.direction, direction);
			}
		);
	};
	auto delay_generator = [test_request](const std::size_t& timeout_ms) {
		ASSERT_EQ(test_request.step_duration_ms, timeout_ms);
	};

	// WHEN
	StepperHost instance(raw_data_reader, raw_data_writer, stepper_motor_creator, delay_generator);

	// THEN
	ASSERT_NO_THROW(instance.run_once());
}