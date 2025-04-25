#include <iostream>
#include <stdexcept>

#include "gtest/gtest.h"
#include "json/value.h"

#include "movement_host.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "test_ipc_data_reader.hpp"
#include "test_ipc_data_writer.hpp"

using namespace ipc;
using namespace manager;
using namespace host;

TEST(ut_movement_host, ctor_dtor_sanity) {
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
	auto axes_controller = [](const AxisStep& step) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);

	// WHEN
	MovementHost *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new MovementHost(raw_data_reader, raw_data_writer, axes_controller, axes_properties));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_host, run_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);
	const auto request_data = MovementManagerRequest::LinearMovementData {
		.destination = Vector<double>(1.0, 2.0, 3.0),
		.speed = 4.0
	};
	auto request = MovementManagerRequest(request_data);
	const auto expected_response = MovementManagerResponse {
		MovementManagerResponse::ResultCode::OK,
		std::nullopt
	};
	const auto raw_data_writer = TestIpcDataWriter<RawData>(
		[expected_response](const RawData& raw_data) {
			std::cout << "test raw data writer received the following data to write:" << std::endl << std::string(raw_data.begin(), raw_data.end()) << std::endl;
			auto json_response = Json::Value();
			auto reader = Json::Reader();
			ASSERT_TRUE(reader.parse(std::string(raw_data.begin(), raw_data.end()), std::ref(json_response)));
			const auto response = json_value_to_movement_response(json_response);
			ASSERT_EQ(expected_response.code, response.code);
		}
	);
	const auto raw_data_reader = TestIpcDataReader<RawData>(
		[request]() -> std::optional<RawData> {
			const auto json_request = movement_request_to_json_value(request);
			const auto serial_request = Json::FastWriter().write(json_request);
			std::cout << "test raw data reader generated the following raw request data:" << std::endl << serial_request << std::endl;
			return RawData(serial_request.begin(), serial_request.end());
		}
	);
	auto axes_controller = [](const AxisStep& step) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	MovementHost instance(raw_data_reader, raw_data_writer, axes_controller, axes_properties);

	// THEN
	ASSERT_NO_THROW(instance.run_once());
}