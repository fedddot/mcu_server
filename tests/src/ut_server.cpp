#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "test_ipc_connection.hpp"
#include "test_resource.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>

using namespace server;

TEST(ut_server, ctor_cctor_dtor_sanity) {
    // GIVEN
    const server_uts::TestResource test_vendor(
        [](const Request&)-> Response {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );
    server_uts::TestIpcConnection connection(
        [](const Response&)-> void {
            throw std::runtime_error("NOT IMPLEMENTED");
        }
    );

    // WHEN
    std::unique_ptr<Server> instance_ptr(nullptr);
    std::unique_ptr<Server> copy_instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = std::unique_ptr<Server>(new Server(&connection, test_vendor)));
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(copy_instance_ptr = std::unique_ptr<Server>(new Server(*instance_ptr)));
    ASSERT_NE(nullptr, instance_ptr);
    
    ASSERT_NO_THROW(instance_ptr = nullptr);
    ASSERT_NO_THROW(copy_instance_ptr = nullptr);
}