#include "gtest/gtest.h"

#include "json_ipc_data_writer.hpp"

using namespace ipc;

using TestResponse = int;

TEST(ut_json_ipc_data_writer, ctor_dtor_sanity) {
	// GIVEN
	
	// WHEN
	JsonIpcDataWriter<TestResponse> *instance = nullptr;

	// THEN
	ASSERT_NO_THROW(instance = new JsonIpcDataWriter<TestResponse>());
	ASSERT_NO_THROW(delete instance);
	instance = nullptr;
}

TEST(ut_json_ipc_data_writer, write_sanity) {
	// GIVEN
	const auto response = TestResponse(4);
	
	// WHEN
	auto instance = JsonIpcDataWriter<TestResponse>();

	// THEN
	ASSERT_NO_THROW(instance.write(response));
}