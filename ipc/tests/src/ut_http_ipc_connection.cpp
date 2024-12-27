#include <string>

#include "gtest/gtest.h"
#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"
#include "pplx/pplxtasks.h"

#include "http_ipc_connection.hpp"
#include "json_data_serializer.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_types.hpp"
#include "string.hpp"

using namespace ipc;
using namespace server;

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

TEST(ut_http_ipc_connection, write_readable_read_sanity) {
    // GIVEN
    const auto test_url = "http://127.0.0.1:5000";
    const auto polling_timeout = 1;
    const auto response_timeout = 3;
    auto test_body = Body();
    test_body.add("key", String("val"));
    const auto test_request = server::Request(server::Request::Method::DELETE, {"one", "two"}, test_body);

    // WHEN
    HttpIpcConnection instance(test_url, polling_timeout, response_timeout);

    // THEN
    ASSERT_FALSE(instance.readable());

    // send test request
    web::http::http_request test_http_request(web::http::methods::DEL);
    test_http_request.set_request_uri("/one/two");
    test_http_request.set_body(
        server_utl::JsonDataSerializer()(test_body),
        utf8string("application/json; charset=utf-8")
    );
    web::http::client::http_client client(test_url);
    auto request_task = client.request(test_http_request);
    
    ASSERT_TRUE(instance.readable());
    ASSERT_NO_THROW(instance.write(Response(ResponseCode::BAD_REQUEST, Body())));
    ASSERT_EQ(request_task.wait(), pplx::task_status::completed);
}