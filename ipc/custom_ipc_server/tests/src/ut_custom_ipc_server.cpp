#include "gtest/gtest.h"
#include <stdexcept>
#include <vector>

#include "custom_ipc_server.hpp"

using namespace ipc;

using TestRequest = std::string;
using TestResponse = int;
using TestRawData = std::vector<char>;

using TestServer = CustomIpcServer<TestRequest, TestResponse, TestRawData>;

TEST(ut_custom_ipc_server, ctor_dtor_sanity) {
    // GIVEN
    auto capturer = [](const TestRawData&)-> bool {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    auto reader = [](TestRawData *)-> TestRequest {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    auto writer = [](const TestResponse&) {
        throw std::runtime_error("NOT IMPLEMENTED");
    };

    // WHEN
    auto test_buffer = TestRawData();
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestServer(
            &test_buffer,
            capturer,
            reader,
            writer
        )
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}