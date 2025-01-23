#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "gtest/gtest.h"

#include "buffered_ipc_server.hpp"
#include "ipc_option.hpp"

using namespace ipc;

using TestRequest = std::string;
using TestResponse = int;

using TestServer = BufferedIpcServer<TestRequest, TestResponse>;

TEST(ut_buffered_ipc_server, ctor_dtor_sanity) {
    // WHEN
    TestServer *instance_ptr(nullptr);

    // THEN
    ASSERT_NO_THROW(
        instance_ptr = new TestServer(
            [](std::vector<char> *buffer) -> Option<TestRequest> {
                throw std::runtime_error("NOT_IMPLEMENTED");
            },
			[](const TestResponse& response) {
                throw std::runtime_error("NOT_IMPLEMENTED");
            },
			2UL
        )
    );
    ASSERT_NO_THROW(delete instance_ptr);
    instance_ptr = nullptr;
}

TEST(ut_buffered_ipc_server, feed_read_sanity) {
    // GIVEN
    const auto test_request = TestRequest("test_request");
    const auto test_response = TestResponse(12);
    auto mux = std::mutex();
    auto cond = std::condition_variable();
    auto handler = [test_request, test_response, &mux, &cond](const TestRequest& request) {
        auto lock = std::unique_lock(mux);
        if (test_request != request) {
            throw std::runtime_error("request assertion failed");
        }
        cond.notify_one();
        return test_response;
    };

    // WHEN
    auto instance = TestServer(
        [test_request](std::vector<char> *buffer) -> Option<TestRequest> {
            if (test_request.size() > buffer->size()) {
                return Option<TestRequest>(nullptr);
            }
            const auto request_start = buffer->begin();
            const auto request_end = request_start + test_request.size();
            const auto request = TestRequest(request_start, request_end);
            buffer->erase(request_start, request_end);
            return Option<TestRequest>(new TestRequest(request));
        },
        [test_response](const TestResponse& response) {
            if (test_response != response) {
                throw std::runtime_error("response assertion failed");
            }
        },
        2UL
    );

    // THEN
    auto serving_thread = std::thread(
        [&instance, &handler](void) {
            ASSERT_NO_THROW(instance.serve(handler));
        }
    );
    auto iter = test_request.begin();
    while (test_request.end() != iter) {
        ASSERT_NO_THROW(instance.feed(*iter));
        ++iter;
    }

    auto lock = std::unique_lock(mux);
    cond.wait(lock);
    ASSERT_NO_THROW(instance.stop());
    serving_thread.join();

}