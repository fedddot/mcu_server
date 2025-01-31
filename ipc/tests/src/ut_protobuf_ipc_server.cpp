#include "gtest/gtest.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "pb.h"
#include "stepper_motor_service.pb.h"
#include "pb_common.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "protobuf_ipc_server.hpp"

bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
    const auto string_casted = static_cast<const char *>(*arg);
    const auto string_size = std::strlen(string_casted);
    return pb_encode_string(stream, static_cast<const pb_byte_t *>(*arg), string_size);
}

TEST(ut_protobuf_ipc_server, ctor_dtor_sanity) {
    const auto test_motor_id = std::string("test_motor");
    pb_byte_t buff[1024] = {'\0'};

    stepper_motor_service_StepperMotorRequest request = stepper_motor_service_StepperMotorRequest_init_default;
    request.motor_id.arg = new char[test_motor_id.size() + 1];
    std::strcpy(static_cast<char *>(request.motor_id.arg), test_motor_id.c_str());
    request.motor_id.funcs.encode = encode_string;

    auto stream = pb_ostream_from_buffer(buff, 1024);


    if (!pb_encode(&stream, stepper_motor_service_StepperMotorRequest_fields, &request)) {
        throw std::runtime_error("failed to encode");
    }
    std::cout << buff << std::endl;
}