#include <condition_variable>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "cpprest/http_client.h"
#include "cpprest/http_msg.h"

#include "http_ipc_server.hpp"
#include "ipc_request_handler.hpp"

using namespace ipc;

using TestRequest = std::string;
using TestResponse = int;

using TestServer = HttpIpcServer<TestRequest, TestResponse>;

TEST(ut_http_ipc_server, ctor_dtor_sanity) {
    // GIVEN
    const auto uri = std::string("http://127.0.0.1:5000");
    auto to_request_transformer = [](const web::http::http_request& request) {
        const auto data = request.extract_vector().get();
        return TestRequest(data.begin(), data.end());
    };
    auto to_response_transformer = [](const TestResponse& response) {
        auto http_response = web::http::http_response(web::http::status_codes::OK);
        http_response.set_body(std::to_string(response));
        return http_response;
    };

    // WHEN
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestServer(
            uri,
            to_request_transformer,
            to_response_transformer
        )
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

class TestHandler: public IpcRequestHandler<TestRequest, TestResponse> {
public:
    TestHandler(const std::function<TestResponse(const TestRequest&)>& action): m_action(action) {

    }
    TestResponse handle(const TestRequest& request) const override {
        return m_action(request);
    }
private:
    std::function<TestResponse(const TestRequest&)> m_action;
};

TEST(ut_http_ipc_server, write_read_sanity) {
    // GIVEN
    const auto uri = std::string("http://127.0.0.1:5000");
    auto to_request_transformer = [](const web::http::http_request& request) {
        const auto data = request.extract_vector().get();
        return TestRequest(data.begin(), data.end());
    };
    auto to_response_transformer = [](const TestResponse& response) {
        auto http_response = web::http::http_response(web::http::status_codes::OK);
        http_response.set_body(std::to_string(response));
        return http_response;
    };
    const auto test_http_request_body = std::string("booboo");
    auto test_http_request = web::http::http_request(web::http::methods::POST);
    test_http_request.set_request_uri("/one/two");
    test_http_request.set_body(
        test_http_request_body,
        utf8string("application/json; charset=utf-8")
    );

    auto mux = std::mutex();
    auto cond = std::condition_variable();


    // WHEN
    auto instance = TestServer(
        uri,
        to_request_transformer,
        to_response_transformer
    );
    auto handler = TestHandler(
        [](const TestRequest& request) {
            std::cout << "received request: " << request << std::endl;
            return 0;
        }
    );
  
    // THEN
    auto serving_thread = std::thread(
        [](TestServer *server, IpcRequestHandler<TestRequest, TestResponse> *handler) {
            server->serve(handler);
        },
        &instance,
        &handler
    );

    web::http::client::http_client client(uri);
    auto result = client.request(test_http_request).get();
    ASSERT_EQ(web::http::status_codes::OK, result.status_code());
    instance.stop();
    serving_thread.join();
}