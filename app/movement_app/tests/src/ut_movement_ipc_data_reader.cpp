#include <optional>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/writer.h"

#include "ipc_instance.hpp"
#include "linear_movement_request.hpp"
#include "movement_ipc_data_infra.hpp"
#include "movement_ipc_data_reader.hpp"
#include "movement_manager_request.hpp"
#include "test_ipc_data_reader.hpp"

using namespace ipc;
using namespace manager;

using AxisControllerConfig = std::string;

static Json::Value cfg2json(const AxisControllerConfig& cfg);
static AxisControllerConfig json2cfg(const Json::Value& cfg);

TEST(ut_movement_ipc_data_reader, ctor_dtor_sanity) {
	// GIVEN
	const auto test_raw_data_reader = TestIpcDataReader<RawData>(
		[]() -> std::optional<Result<RawData>> {
			return std::nullopt;
		}
	);
	
	// WHEN
	MovementIpcDataReader<AxisControllerConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(
		instance = new MovementIpcDataReader<AxisControllerConfig>(
			test_raw_data_reader,
			cfg2json,
			json2cfg
		)
	);
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_movement_ipc_data_reader, read_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);
	const auto request = LinearMovementRequest(Vector<double>(1.0, 2.0, 3.0), 4.0);
	const auto transformers = DefaultMovementDataTransformers<AxisControllerConfig>(cfg2json, json2cfg);
	auto json_movement_request = transformers.request_to_json_value(request);
	const auto serial_json_movement_request = Json::writeString(Json::StreamWriterBuilder(), json_movement_request);

	const auto test_raw_data_reader = TestIpcDataReader<RawData>(
		[serial_json_movement_request]() -> std::optional<Result<RawData>> {
			return Result(new RawData(serial_json_movement_request.begin(), serial_json_movement_request.end()));
		}
	);
	
	// WHEN
	MovementIpcDataReader<AxisControllerConfig> instance(test_raw_data_reader, cfg2json, json2cfg);
	auto read_result = std::optional<Result<MovementManagerRequest>>();

	// THEN
	ASSERT_NO_THROW(read_result = instance.read());
	ASSERT_TRUE(read_result);
	ASSERT_EQ(request.type(), read_result->get().type());
}

inline Json::Value cfg2json(const AxisControllerConfig& cfg) {
	throw std::runtime_error("NOT IMPLEMENTED");
}

inline AxisControllerConfig json2cfg(const Json::Value& cfg) {
	throw std::runtime_error("NOT IMPLEMENTED");
}