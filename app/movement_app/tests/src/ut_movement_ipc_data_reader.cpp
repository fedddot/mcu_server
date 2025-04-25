#include <string>

#include "gtest/gtest.h"
#include "json/writer.h"

#include "movement_ipc_data_infra.hpp"
#include "movement_ipc_data_reader.hpp"
#include "movement_manager_request.hpp"
#include "test_ipc_data_reader.hpp"

using namespace ipc;
using namespace manager;

TEST(ut_movement_ipc_data_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto test_raw_data_reader = TestIpcDataReader<RawData>(
		[]() -> std::optional<RawData> {
			return std::optional<RawData>();
		}
	);
	
	// WHEN
	MovementIpcDataReader *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new MovementIpcDataReader(test_raw_data_reader));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_ipc_data_reader, read_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);
	const auto request_data = MovementManagerRequest::LinearMovementData {
		.destination = Vector<double>(1.0, 2.0, 3.0),
		.speed = 4.0
	};
	auto request = MovementManagerRequest(request_data);
	auto json_movement_request = movement_request_to_json_value(request);
	const auto serial_json_movement_request = Json::writeString(Json::StreamWriterBuilder(), json_movement_request);

	const auto test_raw_data_reader = TestIpcDataReader<RawData>(
		[serial_json_movement_request]() -> std::optional<RawData> {
			return std::optional<RawData>(RawData(serial_json_movement_request.begin(), serial_json_movement_request.end()));
		}
	);
	
	// WHEN
	MovementIpcDataReader instance(test_raw_data_reader);
	auto read_result = std::optional<MovementManagerRequest>();

	// THEN
	ASSERT_NO_THROW(read_result = instance.read());
	ASSERT_TRUE(read_result);
	ASSERT_EQ(request.get_movement_type(), read_result->get_movement_type());
}