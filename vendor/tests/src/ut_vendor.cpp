#include "gtest/gtest.h"
#include <memory>
#include <stdexcept>

#include "request.hpp"
#include "response.hpp"
#include "string.hpp"
#include "test_resource.hpp"
#include "vendor.hpp"

using namespace server;
using namespace server_uts;

TEST(ut_vendor, ctor_cctor_clone_dtor_id_sanity) {
	// GIVEN
	const std::string test_vendor_id("test_vendor_id");
	
	// WHEN
	using VendorUnqPtr = std::unique_ptr<Vendor>;
	using ResourceUnqPtr = std::unique_ptr<Resource>;
	VendorUnqPtr instance_ptr(nullptr);
	VendorUnqPtr instance_ptr_copy(nullptr);
	ResourceUnqPtr instance_ptr_clone(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = VendorUnqPtr(new Vendor()));
	ASSERT_NE(nullptr, instance_ptr);

	// cctor
	ASSERT_NO_THROW(instance_ptr_copy = VendorUnqPtr(new Vendor(*instance_ptr)));
	ASSERT_NE(nullptr, instance_ptr_copy);

	// clone
	ASSERT_NO_THROW(instance_ptr_clone = ResourceUnqPtr(instance_ptr->clone()));
	ASSERT_NE(nullptr, instance_ptr_clone);

	// dtors
	ASSERT_NO_THROW(instance_ptr = nullptr);
	ASSERT_NO_THROW(instance_ptr_copy = nullptr);
	ASSERT_NO_THROW(instance_ptr_clone = nullptr);
}

TEST(ut_vendor, register_resource_contains_resource_sanity) {
	// GIVEN
	const std::string test_vendor_id("test_vendor_id");
	const std::string test_resource_id_1("test_resource_id_1");
	const std::string test_resource_id_2("test_resource_id_2");
	auto stub = [](const Request&) -> Response {
		throw std::runtime_error("NOT_IMPLEMENTED");
	};
	const TestResource test_resource_1(stub);
	const TestResource test_resource_2(stub);
	
	// WHEN
	Vendor instance;

	// THEN
	ASSERT_FALSE(instance.contains_resource(test_resource_id_1));
	ASSERT_NO_THROW(instance.register_resource(test_resource_id_1, test_resource_1));
	ASSERT_TRUE(instance.contains_resource(test_resource_id_1));

	ASSERT_FALSE(instance.contains_resource(test_resource_id_2));
	ASSERT_NO_THROW(instance.register_resource(test_resource_id_2, test_resource_2));
	ASSERT_TRUE(instance.contains_resource(test_resource_id_2));
}

TEST(ut_vendor, run_request_sanity) {
	// GIVEN
	const std::string test_vendor_id("test_vendor_id");
	
	using ResponseCode = typename Response::ResponseCode;
	const std::string test_resource_id_1("test_resource_id_1");
	const auto test_response_code_1(ResponseCode::OK);
	
	const std::string test_resource_id_2("test_resource_id_2");
	const auto test_response_code_2(ResponseCode::NOT_FOUND);

	const TestResource test_resource_1(
		[test_resource_id_1, test_response_code_1](const Request&) -> Response {
			Response::Body data;
			data.add("resourse_id", String(test_resource_id_1));
			return Response(test_response_code_1, data);
		}
	);
	const TestResource test_resource_2(
		[test_resource_id_2, test_response_code_2](const Request&) -> Response {
			Response::Body data;
			data.add("resourse_id", String(test_resource_id_2));
			return Response(test_response_code_2, data);
		}
	);
	Request test_request_1(Request::Method::CREATE, {test_resource_id_1}, Request::Body());
	Request test_request_2(Request::Method::READ, {test_resource_id_2}, Request::Body());
	
	// WHEN
	Vendor instance;
	instance.register_resource(test_resource_id_1, test_resource_1);
	instance.register_resource(test_resource_id_2, test_resource_2);
	std::unique_ptr<Response> response(nullptr);

	// THEN
	ASSERT_NO_THROW(response = std::make_unique<Response>(instance.run_request(test_request_1)));
	ASSERT_EQ(test_response_code_1, response->code());
	ASSERT_NO_THROW(response = std::make_unique<Response>(instance.run_request(test_request_2)));
	ASSERT_EQ(test_response_code_2, response->code());
}