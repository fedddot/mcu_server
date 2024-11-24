#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "resources_vendor.hpp"
#include "response.hpp"
#include "test_ipc_connection.hpp"
#include "custom_server.hpp"

using namespace server;
using namespace server_utl;
using namespace ipc_uts;
using namespace vendor;

TEST(ut_custom_server, ctor_dtor_sanity) {
    // GIVEN
    const std::string test_id("test_server");
    ResourcesVendor vendor;
    TestIpcConnection<std::string> connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    CustomServer<std::string> *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new CustomServer<std::string>(&connection, test_id, &vendor));
    ASSERT_NE(nullptr, instance_ptr);
    ASSERT_NO_THROW(delete instance_ptr);
}