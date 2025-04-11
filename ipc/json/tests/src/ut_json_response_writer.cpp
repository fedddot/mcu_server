#include "gtest/gtest.h"

#include "json_response_writer.hpp"

using namespace ipc;

using TestResponse = int;

TEST(ut_json_response_writer, ctor_dtor_sanity) {
	// GIVEN
	
	// WHEN
	JsonResponseWriter<TestResponse> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new JsonResponseWriter<TestResponse>());
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_json_response_writer, write_sanity) {
	// GIVEN
	const auto response = TestResponse(4);
	
	// WHEN
	auto instance = JsonResponseWriter<TestResponse>();

	// THEN
	ASSERT_NO_THROW(instance.write(response));
}