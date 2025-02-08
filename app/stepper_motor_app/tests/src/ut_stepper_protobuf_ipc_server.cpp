#include <cstring>
#include <vector>

#include "gtest/gtest.h"

#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_protobuf_ipc_server.hpp"

#define BUFF_SIZE 128UL

using namespace ipc;
using namespace manager;

using CreateConfig = int;
using Request = StepperMotorRequest<CreateConfig>;
using Response = StepperMotorResponse;

static std::vector<char> serialize_request(const Request& request);

TEST(ut_stepper_protobuf_ipc_server, ctor_dtor_sanity) {
    // WHEN
    StepperProtobufIpcServer<CreateConfig, BUFF_SIZE> *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        (
            instance_ptr = new StepperProtobufIpcServer<CreateConfig, BUFF_SIZE>(
                [](const std::vector<char>&) {
                    throw std::runtime_error("NOT IMPLEMENTED");
                }
            )
        )
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

TEST(ut_stepper_protobuf_ipc_server, feed_serve_sanity) {
    // GIVEN
    auto request = Request(
        Request::Type::CREATE_STEPPER,
        "test_motor"
    );

    // WHEN
    auto handler = [](const Request& request)-> Response {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    StepperProtobufIpcServer<CreateConfig, BUFF_SIZE> instance(
        [](const std::vector<char>&) {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );
    const auto serial_request = serialize_request(request);

    // THEN
    for (const auto ch: serial_request) {
        ASSERT_NO_THROW(instance.feed(ch));
    }
    ASSERT_NO_THROW(instance.serve_once(handler));
}

inline std::vector<char> serialize_request(const Request& request) {
    
}