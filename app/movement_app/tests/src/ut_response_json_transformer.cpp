#include <cstdlib>

#include "gtest/gtest.h"
#include "json/value.h"

#include "response_json_transformer.hpp"
#include "movement_manager_response.hpp"

using namespace ipc;
using namespace manager;

TEST(ut_response_json_transformer, ctor_dtor_sanity) {
	// WHEN
	ResponseJsonTransformer *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new ResponseJsonTransformer());
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_response_json_transformer, response2json_sanity) {
	// GIVEN
	const auto response = MovementManagerResponse {
		.code = MovementManagerResponse::ResultCode::BAD_REQUEST,
		.message = "test_message",
	};
	
	// WHEN
	ResponseJsonTransformer instance;
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
	ResponseJsonTransformer instance;
	const auto test_response_json = instance.response_to_json_value(test_response);
	auto result = MovementManagerResponse(test_response);

	// THEN
	ASSERT_NO_THROW(result = instance.json_value_to_response(test_response_json));
	ASSERT_EQ(test_response.code, result.code);
	ASSERT_EQ(test_response.message, result.message);
}