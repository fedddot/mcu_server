#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "buffered_ipc_server.hpp"
#include "ipc_option.hpp"

using namespace ipc;

using TestRequest = std::string;
using TestResponse = int;

using TestServerConfig = BufferedIpcServerConfig<TestRequest, TestResponse>;
using TestServer = BufferedIpcServer<TestRequest, TestResponse>;

TEST(ut_buffered_ipc_server, ctor_dtor_sanity) {
    // GIVEN
    auto test_cfg = TestServerConfig();
    test_cfg.request_reader = [](RawData *buffer)-> Option<TestRequest> {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    test_cfg.response_writer = [](const TestResponse& response) {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    test_cfg.realloc_size = 2UL;

    // WHEN
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new TestServer(test_cfg));
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

TEST(ut_buffered_ipc_server, feed_read_sanity) {
    // GIVEN
    const auto test_request = TestRequest("test_request");

    auto test_cfg = TestServerConfig();
    test_cfg.request_reader = [&test_request](RawData *buffer)-> Option<TestRequest> {
        if (test_request.size() > buffer->size()) {
            return Option<TestRequest>(nullptr);
        }
        const auto request_start = buffer->begin();
        const auto request_end = request_start + test_request.size();
        const auto request = TestRequest(request_start, request_end);
        buffer->erase(request_start, request_end);
        return Option<TestRequest>(new TestRequest(request));
    };
    test_cfg.response_writer = [](const TestResponse& response) {
        throw std::runtime_error("NOT IMPLEMENTED");
    };
    test_cfg.realloc_size = 2UL;

    // WHEN
    TestServer instance(test_cfg);
    Option<TestRequest> result(nullptr);

    // THEN
    const auto last_request_char_iter = test_request.begin() + test_request.size() - 1;
    auto char_iter = test_request.begin();
    while (last_request_char_iter != char_iter) {
        ASSERT_NO_THROW(instance.feed(*char_iter));
        ASSERT_NO_THROW(result = instance.read());
        ASSERT_FALSE(result.some());
        ++char_iter;
    }
    ASSERT_NO_THROW(instance.feed(*char_iter));
    ASSERT_NO_THROW(result = instance.read());
    ASSERT_TRUE(result.some());
    ASSERT_EQ(test_request, result.get());

    ASSERT_NO_THROW(result = instance.read());
    ASSERT_FALSE(result.some());
}