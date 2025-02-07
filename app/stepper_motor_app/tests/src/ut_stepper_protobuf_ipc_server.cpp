#include <cstring>
#include <vector>

#include "gtest/gtest.h"

#include "stepper_protobuf_ipc_server.hpp"

#define BUFF_SIZE 128UL

using namespace ipc;
using CreateConfig = int;

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
    // WHEN
    StepperProtobufIpcServer<CreateConfig, BUFF_SIZE> instance(
        [](const std::vector<char>&) {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // THEN
    
}