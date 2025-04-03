#include <string>

#include "gtest/gtest.h"

#include "pb_request_reader.hpp"

using namespace ipc;

TEST(ut_pb_request_reader, sanity) {
	// GIVEN

	// WHEN
	ProtobufRequestReader<std::string> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new ProtobufRequestReader<std::string>());
}