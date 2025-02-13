#include "gtest/gtest.h"

#include "custom_ipc_server.hpp"

using namespace ipc;

using TestRequest = std::string;
using TestResponse = int;

using TestServer = CustomIpcServer<TestRequest, TestResponse>;

TEST(ut_custom_ipc_server, ctor_dtor_sanity) {
    // GIVEN

    // WHEN
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestServer()
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}