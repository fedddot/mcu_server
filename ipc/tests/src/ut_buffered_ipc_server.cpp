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