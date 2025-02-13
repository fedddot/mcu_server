#include <iostream>
#include <stdexcept>
#include <vector>

#include "gtest/gtest.h"

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

TEST(ut_custom_ipc_server, run_once_sanity) {
    // GIVEN
    const auto test_msg_size = 3UL;
    const auto test_msg1 = std::string("ABC");
    const auto test_msg2 = std::string("DEF");

    auto capturer = [test_msg_size](const TestRawData& data)-> bool {
        return data.size() >= test_msg_size;
    };
    auto reader = [test_msg_size](TestRawData *data)-> TestRequest {
        if (data->size() < test_msg_size) {
            throw std::runtime_error("insufficient buffer");
        }
        const auto msg_start = data->begin();
        const auto msg_end = msg_start + test_msg_size;;
        const auto extracted_request = TestRequest(msg_start, msg_end);
        data->erase(msg_start, msg_end);
        return extracted_request;
    };
    auto writer = [](const TestResponse& response) {
        std::cout << "response received: " << response << std::endl;
    };

    // WHEN
    auto test_buffer = TestRawData();
    auto callback_invoked = false;
    auto callback = [&callback_invoked](const TestRequest& request) {
        std::cout << "callback has been invoked with request: " << request << std::endl;
        callback_invoked = true;
        return TestResponse(0); 
    };
    TestServer instance(
        &test_buffer,
        capturer,
        reader,
        writer
    );

    // THEN
    // make sure when the buffer is empty the cb won't be invoked
    ASSERT_NO_THROW(instance.serve_once(callback));
    ASSERT_FALSE(callback_invoked);

    // make sure when the buffer contains a message the cb will be invoked
    test_buffer.insert(test_buffer.end(), test_msg1.begin(), test_msg1.end());
    ASSERT_NO_THROW(instance.serve_once(callback));
    ASSERT_TRUE(callback_invoked);
    callback_invoked = false;

    // make sure when the buffer is empty the cb won't be invoked
    ASSERT_NO_THROW(instance.serve_once(callback));
    ASSERT_FALSE(callback_invoked);

    // make sure when the buffer contains a message the cb will be invoked
    test_buffer.insert(test_buffer.end(), test_msg2.begin(), test_msg2.end());
    ASSERT_NO_THROW(instance.serve_once(callback));
    ASSERT_TRUE(callback_invoked);
    callback_invoked = false;
}