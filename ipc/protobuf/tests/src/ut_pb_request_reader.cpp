#include <string>

#include "gtest/gtest.h"

#include "pb.h"
#include "pb_decode.h"
#include "pb_request_reader.hpp"
#include "example.pb.h"

using namespace ipc;

struct TestMessage {
	std::string msg;
};

TEST(ut_pb_request_reader, ctor_dtor_sanity) {
	// GIVEN
	enum: int { BUFF_SIZE = 10 };
	pb_byte_t buff[BUFF_SIZE] = { '\0' };
	auto stream = pb_istream_from_buffer(buff, BUFF_SIZE);
	
	// WHEN
	ProtobufRequestReader<TestMessage> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new ProtobufRequestReader<TestMessage>(&stream, example_ExampleRequest_fields));
	ASSERT_NO_THROW(delete instance_ptr);
}

TEST(ut_pb_request_reader, read_sanity) {
	// GIVEN
	enum: int { BUFF_SIZE = 10 };
	pb_byte_t buff[BUFF_SIZE] = { '\0' };
	auto stream = pb_istream_from_buffer(buff, BUFF_SIZE);
	
	// WHEN
	ProtobufRequestReader<TestMessage> instance(&stream, example_ExampleRequest_fields);
	auto result = std::optional<TestMessage>();

	// THEN
	ASSERT_NO_THROW(result = instance.read());
}