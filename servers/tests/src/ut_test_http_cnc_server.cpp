#include <string>

#include "gtest/gtest.h"

#include "test_http_cnc_server.hpp"

using namespace server;
using namespace cnc_server;

TEST(ut_test_http_cnc_server, ctor_dtor_sanity) {
    // GIVEN
    const std::string test_id("test_http_cnc_server");
    const std::string uri("http://127.0.0.1:5000");
    const unsigned int timeout_s(20);
    
    // WHEN
    TestHttpCncServer<std::string> *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new TestHttpCncServer<std::string>(test_id, uri, timeout_s));
    ASSERT_NE(nullptr, instance_ptr);

    instance_ptr->run();
    while (true) {
        ;
    }
    instance_ptr->stop();

    ASSERT_NO_THROW(delete instance_ptr);
}
