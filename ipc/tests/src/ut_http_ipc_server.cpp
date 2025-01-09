#include "gtest/gtest.h"

#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"

#include "http_ipc_server.hpp"

using namespace ipc;
using TestServerConfig = HttoIpcServerConfig<web::http::http_request, web::http::http_response>;
using TestServer = HttpIpcServer<web::http::http_request, web::http::http_response>;

TEST(ut_http_ipc_server, ctor_dtor_sanity) {
    // GIVEN
    auto test_cfg = TestServerConfig();
    test_cfg.uri = "http://127.0.0.1:5000";
    test_cfg.polling_timeout_s = 3;
    test_cfg.response_timeout_s = 3;
    test_cfg.to_request = [](const web::http::http_request& request) {
        return request;
    };
    test_cfg.to_response = [](const web::http::http_response& response) {
        return response;
    };

    // WHEN
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(instance_ptr = new TestServer(test_cfg));
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

TEST(ut_http_ipc_server, write_readable_read_sanity) {
    // GIVEN
    auto test_cfg = TestServerConfig();
    test_cfg.uri = "http://127.0.0.1:5000";
    test_cfg.polling_timeout_s = 3;
    test_cfg.response_timeout_s = 3;
    test_cfg.to_request = [](const web::http::http_request& request) {
        return request;
    };
    test_cfg.to_response = [](const web::http::http_response& response) {
        return response;
    };
    const auto test_request = web::http::http_request(web::http::methods::GET);
    const auto test_response = web::http::http_response(web::http::status_codes::BadRequest);

    // WHEN
    TestServer instance(test_cfg);
    web::http::http_request read_result;

    // THEN
    ASSERT_FALSE(instance.readable());

    // send test request
    web::http::client::http_client client(test_cfg.uri);
    auto request_task = client.request(test_request);
    
    ASSERT_TRUE(instance.readable());
    ASSERT_NO_THROW(read_result = instance.read());
    ASSERT_EQ(test_request.method(), read_result.method());
    ASSERT_NO_THROW(instance.write(test_response));
    ASSERT_EQ(request_task.wait(), pplx::task_status::completed);
}