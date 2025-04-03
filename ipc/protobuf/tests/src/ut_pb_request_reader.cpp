#include "gtest/gtest.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <vector>

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
	ASSERT_NO_THROW(
		instance_ptr = new ProtobufRequestReader<example_ExampleRequest>(
			&stream,
			example_ExampleRequest_fields,
			[]() -> example_ExampleRequest {
				return example_ExampleRequest {};
			}
		)
	);
	ASSERT_NO_THROW(delete instance_ptr);
}

bool encode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
	if (!pb_encode_tag_for_field(stream, field)) {
        return false;
	}
	auto str = (const char *)(*arg);
	return pb_encode_string(stream, (const pb_byte_t *)str, std::strlen(str));
}

bool decode(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    uint8_t buffer[1024] = {0};
    
    /* We could read block-by-block to avoid the large buffer... */
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;
    
    /* Print the string, in format comparable with protoc --decode.
     * Format comes from the arg defined in main().
     */
	char *buff = (char *)*arg;
	std::strcpy(buff, (const char *)buffer);
    return true;
}

TEST(ut_pb_request_reader, read_sanity) {
	// GIVEN
	enum: int { BUFF_SIZE = 1024 };
	pb_byte_t buff[BUFF_SIZE] = { '\0' };
	
	auto test_output_stream = pb_ostream_from_buffer(buff, BUFF_SIZE);
	auto test_string_param = "asdf";
	auto test_string = pb_callback_t {
		.funcs{ .encode = encode },
		.arg = const_cast<char *>(test_string_param),
	};
	auto test_request = example_ExampleRequest {
		.enum_param = example_EnumParam_TWO,
		.string_param = test_string
	};
	ASSERT_TRUE(pb_encode_delimited(&test_output_stream, example_ExampleRequest_fields, &test_request));

	auto stream = pb_istream_from_buffer(buff, BUFF_SIZE);
	pb_byte_t buffer[BUFF_SIZE] = { '\0' };

	// WHEN
	ProtobufRequestReader<example_ExampleRequest> instance(
		&stream,
		example_ExampleRequest_fields,
		[buffer]() -> example_ExampleRequest {
			return example_ExampleRequest {
				.string_param = {
					.funcs {
						.decode = decode
					},
					.arg = (void *)buffer
				}
			};
		}
	);
	auto result = std::optional<example_ExampleRequest>();

	// THEN
	ASSERT_NO_THROW(result = instance.read());
	ASSERT_TRUE(result);

	const auto result_casted = *result;
	ASSERT_EQ(test_request.enum_param, result_casted.enum_param);
}