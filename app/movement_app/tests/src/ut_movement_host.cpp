#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/value.h"

#include "movement_host.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "test_axes_controller.hpp"
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
	auto axes_controller = TestAxesController(
		[](const AxisStep& step) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
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
	const auto test_requests = {
		MovementManagerRequest(
			MovementManagerRequest::LinearMovementData {
				.destination = Vector<double>(1.0, 2.0, 3.0),
				.speed = 4.0
			}
		),
		MovementManagerRequest(
			MovementManagerRequest::LinearMovementData {
				.destination = Vector<double>(-1.0, 2.0, 3.0),
				.speed = 4.0
			}
		),
		MovementManagerRequest(
			MovementManagerRequest::LinearMovementData {
				.destination = Vector<double>(0.0, 0.0, -3.0),
				.speed = 4.0
			}
		),
	};
	const auto raw_data_writer = TestIpcDataWriter<RawData>(
		[](const RawData& raw_data) {
			std::cout << "test raw data writer received the following data to write:" << std::endl << std::string(raw_data.begin(), raw_data.end()) << std::endl;
			auto json_response = Json::Value();
			auto reader = Json::Reader();
			ASSERT_TRUE(reader.parse(std::string(raw_data.begin(), raw_data.end()), std::ref(json_response)));
			const auto response = json_value_to_movement_response(json_response);
			ASSERT_EQ(MovementManagerResponse::ResultCode::OK, response.code);
		}
	);
	auto test_requests_iter = test_requests.begin();
	const auto raw_data_reader = TestIpcDataReader<RawData>(
		[&test_requests, &test_requests_iter]() -> std::optional<RawData> {
			if (test_requests.end() == test_requests_iter) {
				return std::nullopt;
			}
			const auto json_request = movement_request_to_json_value(*test_requests_iter);
			const auto serial_request = Json::FastWriter().write(json_request);
			std::cout << "test raw data reader generated the following raw request data:" << std::endl << serial_request << std::endl;
			++test_requests_iter;
			return RawData(serial_request.begin(), serial_request.end());
		}
	);
	auto step_to_str = [](const AxisStep& step) -> std::string {
		const auto axis_mapping = std::map<Axis, std::string> {
			{Axis::X, "X"},
			{Axis::Y, "Y"},
			{Axis::Z, "Z"},
		};
		const auto direction_mapping = std::map<Direction, std::string> {
			{Direction::NEGATIVE, "NEGATIVE"},
			{Direction::POSITIVE, "POSITIVE"},
		};
		std::stringstream stream;
		stream
			<< "step axis " << axis_mapping.at(step.axis) << "; "
			<< "step direction " << direction_mapping.at(step.direction) << "; "
			<< "step duration " << step.duration << "; ";
		return stream.str();
	};
	auto axes_controller = TestAxesController(
		[step_to_str](const AxisStep& step) {
			std::cout << "axes_controller makes step: " << step_to_str(step) << std::endl;
		}
	);

	// WHEN
	MovementHost instance(raw_data_reader, raw_data_writer, axes_controller, axes_properties);

	// THEN
	while (test_requests_iter != test_requests.end()) {
		ASSERT_NO_THROW(instance.run_once());
	}
}