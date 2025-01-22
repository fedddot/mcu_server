#include "gtest/gtest.h"

#include "ipc_server_factory.hpp"

using namespace ipc;

using Request = std::string;
using Response = std::string;
using TestFactory = IpcServerFactory<std::string, std::string, std::string>;

TEST(ut_ipc_server_factory, ctor_dtor_sanity) {
    // WHEN
    TestFactory *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new TestFactory());
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}