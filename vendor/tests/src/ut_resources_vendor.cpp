#include "gtest/gtest.h"
#include <memory>
#include <stdexcept>

#include "object.hpp"
#include "request.hpp"
#include "string.hpp"
#include "resources_vendor.hpp"

using namespace server;
using namespace vendor;

using ResourceId = std::string;

static ResourceId retrieve_manager_id(const Request& request) {
	if (request.path().empty()) {
		throw std::invalid_argument("empty path received");
	}
	return static_cast<ResourceId>(request.path()[0]);
}

static ResourceId retrieve_resource_id(const Request& request) {
	if (Request::Method::CREATE == request.method()) {
		return static_cast<ResourceId>(Data::cast<String>(request.body().access("id")).get());
	}
	if (2UL != request.path().size()) {
		throw std::invalid_argument("wrong path length");
	}
	return static_cast<ResourceId>(request.path()[1]);
}

static Object retrieve_create_data(const Request& request) {
	return Data::cast<Object>(request.body().access("config"));
}

static Object retrieve_update_data(const Request& request) {
	return Data::cast<Object>(request.body().access("config"));
}

TEST(ut_resources_vendor, ctor_dtor_id_sanity) {
	// GIVEN
	const std::string test_vendor_id("test_vendor_id");
	
	// WHEN
	std::unique_ptr<ResourcesVendor<ResourceId>> instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(
		instance_ptr = std::unique_ptr<ResourcesVendor<ResourceId>>(
			new ResourcesVendor<ResourceId>(
				retrieve_manager_id,
				retrieve_resource_id,
				retrieve_create_data,
				retrieve_update_data
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

// TEST(ut_resources_vendor, register_resource_contains_resource_sanity) {
// 	// GIVEN
// 	const std::string test_vendor_id("test_vendor_id");
// 	const std::string test_resource_id_1("test_resource_id_1");
// 	const std::string test_resource_id_2("test_resource_id_2");
// 	auto stub = [](const Request&) -> Response {
// 		throw std::runtime_error("NOT_IMPLEMENTED");
// 	};
// 	const TestResource test_resource_1(stub);
// 	const TestResource test_resource_2(stub);
	
// 	// WHEN
// 	Vendor instance;

// 	// THEN
// 	ASSERT_FALSE(instance.contains_resource(test_resource_id_1));
// 	ASSERT_NO_THROW(instance.register_resource(test_resource_id_1, test_resource_1));
// 	ASSERT_TRUE(instance.contains_resource(test_resource_id_1));

// 	ASSERT_FALSE(instance.contains_resource(test_resource_id_2));
// 	ASSERT_NO_THROW(instance.register_resource(test_resource_id_2, test_resource_2));
// 	ASSERT_TRUE(instance.contains_resource(test_resource_id_2));
// }

// TEST(ut_resources_vendor, run_request_sanity) {
// 	// GIVEN
// 	const std::string test_vendor_id("test_vendor_id");
	
// 	using ResponseCode = typename Response::ResponseCode;
// 	const std::string test_resource_id_1("test_resource_id_1");
// 	const auto test_response_code_1(ResponseCode::OK);
	
// 	const std::string test_resource_id_2("test_resource_id_2");
// 	const auto test_response_code_2(ResponseCode::NOT_FOUND);

// 	const TestResource test_resource_1(
// 		[test_resource_id_1, test_response_code_1](const Request&) -> Response {
// 			Response::Body data;
// 			data.add("resourse_id", String(test_resource_id_1));
// 			return Response(test_response_code_1, data);
// 		}
// 	);
// 	const TestResource test_resource_2(
// 		[test_resource_id_2, test_response_code_2](const Request&) -> Response {
// 			Response::Body data;
// 			data.add("resourse_id", String(test_resource_id_2));
// 			return Response(test_response_code_2, data);
// 		}
// 	);
// 	Request test_request_1(Request::Method::CREATE, {test_resource_id_1}, Request::Body());
// 	Request test_request_2(Request::Method::READ, {test_resource_id_2}, Request::Body());
	
// 	// WHEN
// 	Vendor instance;
// 	instance.register_resource(test_resource_id_1, test_resource_1);
// 	instance.register_resource(test_resource_id_2, test_resource_2);
// 	std::unique_ptr<Response> response(nullptr);

// 	// THEN
// 	ASSERT_NO_THROW(response = std::make_unique<Response>(instance.run_request(test_request_1)));
// 	ASSERT_EQ(test_response_code_1, response->code());
// 	ASSERT_NO_THROW(response = std::make_unique<Response>(instance.run_request(test_request_2)));
// 	ASSERT_EQ(test_response_code_2, response->code());
// }