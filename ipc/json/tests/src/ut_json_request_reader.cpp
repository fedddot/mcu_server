#include <string>

#include "gtest/gtest.h"

#include "json_request_reader.hpp"

using namespace ipc;

using TestRequest = std::string;

TEST(ut_json_request_reader, ctor_dtor_sanity) {
	// GIVEN
	
	// WHEN
	JsonRequestReader<TestRequest> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new JsonRequestReader<TestRequest>());
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_json_request_reader, read_sanity) {
	// GIVEN
	const auto request = TestRequest("request");
	
	// WHEN
	auto instance = JsonRequestReader<TestRequest>();
	auto result = std::optional<TestRequest>();

	// THEN
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_TRUE(result);
}