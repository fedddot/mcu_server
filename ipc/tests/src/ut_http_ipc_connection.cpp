#include <string>

#include "gtest/gtest.h"

#include "http_ipc_connection.hpp"

using namespace ipc;

TEST(ut_http_ipc_connection, ctor_dtor_sanity) {
    // GIVEN
    const auto test_url = "http://127.0.0.1:5000";
    const auto polling_timeout = 3;
    const auto response_timeout = 3;

    // WHEN
    HttpIpcConnection *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new HttpIpcConnection(test_url, polling_timeout, response_timeout));
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}