#include <cstddef>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "buffered_ipc_connection.hpp"
#include "request.hpp"
#include "response.hpp"

using namespace server;
using namespace server_utl;
using namespace ipc;

using RawData = std::string;
using TestConnection = BufferedIpcConnection<int, RawData>;

TEST(ut_buffered_ipc_connection, ctor_dtor_sanity) {
	// GIVEN
	auto matcher = [](const RawData& data)-> bool {
		throw std::runtime_error("NOT_IMPLEMENTED");
	};
	auto extractor = [](RawData *data_ptr)-> Request {
		throw std::runtime_error("NOT_IMPLEMENTED");
	};
	auto sender = [](const Response& response) {
		throw std::runtime_error("NOT_IMPLEMENTED");
	};

	// WHEN
	TestConnection *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new TestConnection(matcher, extractor, sender));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

// TEST(ut_buffered_ipc_connection, feed_sanity) {
// 	// GIVEN
// 	const std::string test_head("test_head");
// 	const std::string test_data("test_data");
// 	const std::string test_tail("test_tail");
// 	const std::size_t test_size(1000UL);
// 	auto sender = [](const std::string& data) {
// 		throw std::runtime_error("NOT IMPLEMENTED");
// 	};

// 	// WHEN
// 	BufferedIpcConnection<std::string> instance(test_head, test_tail, RequestParser(parse_request), ResponseSerializer(serialize_response), sender);
// 	Request request(Request::Method::CREATE, Request::Path{}, Request::Body());
// 	Response response(Response::ResponseCode::OK, Response::Body());

// 	ASSERT_FALSE(instance.readable());
// 	ASSERT_NO_THROW(instance.feed(test_head));
// 	ASSERT_FALSE(instance.readable());
// 	ASSERT_NO_THROW(instance.feed(test_data));
// 	ASSERT_FALSE(instance.readable());
// 	ASSERT_NO_THROW(instance.feed(test_tail));
// 	ASSERT_TRUE(instance.readable());
// 	// ASSERT_NO_THROW(request = instance.read()); TODO: fix
// 	// ASSERT_FALSE(instance.readable()); TODO: fix
// }