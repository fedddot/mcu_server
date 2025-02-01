#include <cstddef>
#include <cstring>
#include <stdexcept>

#include "gtest/gtest.h"
#include <string>
#include "ipc_option.hpp"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

#include "stepper_motor_service.pb.h"
#include "protobuf_ipc_server.hpp"

using namespace ipc;

struct TestRequest {
    std::string motor_id;
};

struct TestResponse {
    enum class ResultCode: int {
        OK,
        FAIL,
    };
    ResultCode code;
};

static const std::size_t test_buff_size(10UL);
using TestIpcServer = ProtobufIpcServer<TestRequest, TestResponse, test_buff_size>;

TEST(ut_protobuf_ipc_server, ctor_dtor_sanity) {
    // GIVEN
    auto request_stream_reader = [](pb_istream_t *) -> Option<TestRequest> {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    auto response_stream_writer = [](const TestResponse&) {
        throw std::runtime_error("NOT IMPLEMENTED");
    };

    // WHEN
    TestIpcServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestIpcServer(
            request_stream_reader,
            response_stream_writer
        )
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
bool decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg);

TEST(ut_protobuf_ipc_server, feed_sanity) {
    // GIVEN
    char buff[1024] = {'\0'};
    auto request_stream_reader = [](pb_istream_t *input_stream) -> Option<TestRequest> {
        auto proto_request = stepper_motor_service_StepperMotorRequest {};
        proto_request.motor_id.funcs.decode = decode_string;
        if (!pb_decode_delimited(input_stream, stepper_motor_service_StepperMotorRequest_fields, &proto_request)) {
            return Option<TestRequest>(nullptr);
        }
        const auto motor_id = std::string((const char *)proto_request.motor_id.arg);
        delete [] (char *)proto_request.motor_id.arg;
        proto_request.motor_id.arg = nullptr;
        return Option<TestRequest>(
            new TestRequest {
                .motor_id = motor_id,
            }
        );
    };
    auto response_stream_writer = [](const TestResponse&) {
        throw std::runtime_error("NOT IMPLEMENTED");
    };

    const auto test_motor_id = std::string("motor");
    auto test_request = stepper_motor_service_StepperMotorRequest {};
    test_request.motor_id.arg = const_cast<char *>(test_motor_id.c_str());
    test_request.motor_id.funcs.encode = encode_string;

    // WHEN
    TestIpcServer instance(
        request_stream_reader,
        response_stream_writer
    );
    pb_byte_t bufff[1024] = {'\0'};
    auto buff_stream = pb_ostream_from_buffer(bufff, 1024);
    ASSERT_TRUE(pb_encode_delimited(&buff_stream, stepper_motor_service_StepperMotorRequest_fields, &test_request));

    // THEN
    int i = 0;
    while ('\0' != bufff[i]) {
        ASSERT_NO_THROW(instance.feed(bufff[i]));
        ++i;
    }
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
    *arg = new char[string_field.size() + 1UL];
    string_field.copy((char *)*arg, string_field.size());
    return true;
}