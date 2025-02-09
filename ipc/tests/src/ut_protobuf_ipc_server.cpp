#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

#include "test_service.pb.h"

#include "ipc_option.hpp"
#include "protobuf_ipc_server.hpp"

using namespace ipc;

struct TestRequest {
    std::string str_field;
    int int_field;
};

struct TestResponse {
    enum class ResultCode: int {
        OK,
        FAIL,
    };
    ResultCode code;
};

static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
static bool decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg);
static Option<TestRequest> read_request_from_pb_stream(pb_istream_t *input_stream);
static void write_test_response(const TestResponse& response);

static const std::size_t test_buff_size(10UL);
using TestIpcServer = ProtobufIpcServer<TestRequest, TestResponse>;

TEST(ut_protobuf_ipc_server, ctor_dtor_sanity) {
    // WHEN
    TestIpcServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestIpcServer(
            read_request_from_pb_stream,
            write_test_response,
            test_buff_size
        )
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

TEST(ut_protobuf_ipc_server, feed_sanity) {
    // GIVEN
    const auto test_str = std::string("str");
    const auto test_int = 179;
    auto test_request = test_service_TestRequest {};
    test_request.str_field.arg = const_cast<char *>(test_str.c_str());
    test_request.str_field.funcs.encode = encode_string;
    test_request.int_field = test_int;
    auto handler_called = false;
    auto test_handler = [test_str, test_int, &handler_called](const TestRequest& request) -> TestResponse {
        handler_called = true;
        if ((test_str != request.str_field) || (test_int != request.int_field)) {
            throw std::runtime_error("ASSERTION FAILED");
        }
        return TestResponse { .code = TestResponse::ResultCode::OK };
    };

    // WHEN
    std::size_t test_request_size(0);
    ASSERT_TRUE(pb_get_encoded_size(&test_request_size, test_service_TestRequest_fields, &test_request));
    test_request_size++;
    auto buff = std::unique_ptr<pb_byte_t[]>(new pb_byte_t[test_request_size]);
    auto buff_stream = pb_ostream_from_buffer(buff.get(), test_request_size);
    ASSERT_TRUE(pb_encode_delimited(&buff_stream, test_service_TestRequest_fields, &test_request));

    TestIpcServer instance(
        read_request_from_pb_stream,
        write_test_response,
        test_buff_size
    );

    // THEN
    int i = 0;
    while ('\0' != (buff.get())[i]) {
        ASSERT_NO_THROW(instance.feed((buff.get())[i]));
        ++i;
    }
    ASSERT_NO_THROW(instance.serve_once(test_handler));
    ASSERT_TRUE(handler_called);
}

inline bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
    const auto string_casted = static_cast<const char *>(*arg);
    const auto string_size = std::strlen(string_casted);
    if (!pb_encode_tag_for_field(stream, field)) {
        return false;
    }
    return pb_encode_string(stream, static_cast<const pb_byte_t *>(*arg), string_size);
}

inline bool decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    auto string_field = std::string("");
    while (stream->bytes_left) {
        pb_byte_t buff = '\0';
        if (!pb_read(stream, &buff, 1UL)) {
            return false;
        }
        if ('\0' == buff) {
            break;
        }
        string_field.push_back(buff);
    }
    string_field.copy((char *)*arg, string_field.size());
    return true;
}

inline Option<TestRequest> read_request_from_pb_stream(pb_istream_t *input_stream) {
    enum: int { MAX_STR_LEN = 0xFF };
    char read_buff[MAX_STR_LEN] = { '\0' };
    auto proto_request = test_service_TestRequest {};
    proto_request.str_field.funcs.decode = decode_string;
    proto_request.str_field.arg = read_buff;
    if (!pb_decode_delimited(input_stream, test_service_TestRequest_fields, &proto_request)) {
        return Option<TestRequest>(nullptr);
    }
    const auto str_field = std::string((const char *)proto_request.str_field.arg);
    return Option<TestRequest>(
        new TestRequest {
            .str_field = str_field,
            .int_field = proto_request.int_field,
        }
    );
}

inline void write_test_response(const TestResponse& response) {
    ASSERT_EQ(TestResponse::ResultCode::OK, response.code);
}