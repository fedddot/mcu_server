#include "gtest/gtest.h"
#include <cstring>
#include <stdexcept>

#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "pb_request_reader.hpp"

#include "example.pb.h"

using namespace ipc;

TEST(ut_pb_request_reader, ctor_dtor_sanity) {
	// GIVEN
	enum: int { BUFF_SIZE = 10 };
	pb_byte_t buff[BUFF_SIZE] = { '\0' };
	auto stream = pb_istream_from_buffer(buff, BUFF_SIZE);
	
	// WHEN
	ProtobufRequestReader<example_ExampleRequest> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new ProtobufRequestReader<example_ExampleRequest>(&stream, example_ExampleRequest_fields));
	ASSERT_NO_THROW(delete instance_ptr);
}

bool encode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
	auto str = (const char *)(*arg);
	return pb_encode_string(stream, (const pb_byte_t *)str, std::strlen(str));
}

TEST(ut_pb_request_reader, read_sanity) {
	// GIVEN
	enum: int { BUFF_SIZE = 1024 };
	pb_byte_t buff[BUFF_SIZE] = { '\0' };
	auto stream = pb_istream_from_buffer(buff, BUFF_SIZE);
	

	auto test_output_stream = pb_ostream_from_buffer(buff, BUFF_SIZE);
	auto test_string_param = "asdf";
	auto test_string = pb_callback_t {
		.funcs{ .encode = encode },
		.arg = const_cast<char *>(test_string_param),
	};
	auto test_request = example_ExampleRequest {
		.enum_param = example_EnumParam_TWO,
		.string_param = test_string,
		.int_param = 4,
	};
	ASSERT_TRUE(pb_encode(&test_output_stream, example_ExampleRequest_fields, &test_request));

	// WHEN
	ProtobufRequestReader<example_ExampleRequest> instance(&stream, example_ExampleRequest_fields);
	auto result = std::optional<example_ExampleRequest>();

	// THEN
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_TRUE(result);
}