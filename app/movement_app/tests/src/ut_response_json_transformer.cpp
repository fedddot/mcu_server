#include <cstdlib>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"
#include "json/value.h"

#include "ipc_instance.hpp"
#include "linear_movement_request.hpp"
#include "response_json_transformer.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_manager_vector.hpp"

using namespace ipc;
using namespace manager;

using AxisControllerConfig = std::string;

static Json::Value cfg2json(const AxisControllerConfig& cfg);
static AxisControllerConfig json2cfg(const Json::Value& cfg);
static bool doubles_equal(const double one, const double other, const double tolerance);

TEST(ut_response_json_transformer, ctor_dtor_sanity) {
	// WHEN
	ResponseJsonTransformer<AxisControllerConfig> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new ResponseJsonTransformer<AxisControllerConfig>(cfg2json, json2cfg));
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_response_json_transformer, linear_request2json_sanity) {
	// GIVEN
	const auto request = LinearMovementRequest(Vector<double>(0.1, 0.2, 0.3), 0.4);
	
	// WHEN
	ResponseJsonTransformer<AxisControllerConfig> instance(cfg2json, json2cfg);
	auto result = Json::Value();

	// THEN
	ASSERT_NO_THROW(result = instance.request_to_json_value(request));
}

TEST(ut_response_json_transformer, json2linear_request_sanity) {
	// GIVEN
	const auto test_request = LinearMovementRequest(Vector<double>(0.1, 0.2, 0.3), 0.4);
	const auto doubles_equality_tolerance = double(0.0001);
	
	// WHEN
	ResponseJsonTransformer<AxisControllerConfig> instance(cfg2json, json2cfg);
	const auto test_request_json = instance.request_to_json_value(test_request);
	auto result = Instance<MovementManagerRequest>(new LinearMovementRequest(test_request));
	const LinearMovementRequest *casted_result_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(result = instance.json_value_to_request(test_request_json));
	casted_result_ptr = dynamic_cast<const LinearMovementRequest *>(&result.get());
	ASSERT_NE(nullptr, casted_result_ptr);
	for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
		ASSERT_TRUE(doubles_equal(test_request.destination().get(axis), casted_result_ptr->destination().get(axis), doubles_equality_tolerance));
	}
	ASSERT_TRUE(doubles_equal(test_request.speed(), casted_result_ptr->speed(), doubles_equality_tolerance));
}

TEST(ut_response_json_transformer, response2json_sanity) {
	// GIVEN
	const auto response = MovementManagerResponse {
		.code = MovementManagerResponse::ResultCode::BAD_REQUEST,
		.message = "test_message",
	};
	
	// WHEN
	ResponseJsonTransformer<AxisControllerConfig> instance(cfg2json, json2cfg);
	auto result = Json::Value();

	// THEN
	ASSERT_NO_THROW(result = instance.response_to_json_value(response));
}

TEST(ut_response_json_transformer, json2response_sanity) {
	// GIVEN
	const auto test_response = MovementManagerResponse {
		.code = MovementManagerResponse::ResultCode::BAD_REQUEST,
		.message = "test_message",
	};
	
	// WHEN
	ResponseJsonTransformer<AxisControllerConfig> instance(cfg2json, json2cfg);
	const auto test_response_json = instance.response_to_json_value(test_response);
	auto result = MovementManagerResponse(test_response);

	// THEN
	ASSERT_NO_THROW(result = instance.json_value_to_response(test_response_json));
	ASSERT_EQ(test_response.code, result.code);
	ASSERT_EQ(test_response.message, result.message);
}

inline Json::Value cfg2json(const AxisControllerConfig& cfg) {
	throw std::runtime_error("NOT IMPLEMENTED");
}

inline AxisControllerConfig json2cfg(const Json::Value& cfg) {
	throw std::runtime_error("NOT IMPLEMENTED");
}

inline bool doubles_equal(const double one, const double other, const double tolerance) {
	return std::abs(one - other) <= std::abs(tolerance);
}