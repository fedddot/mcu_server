#include "gtest/gtest.h"
#include <memory>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "object.hpp"
#include "request.hpp"
#include "response.hpp"
#include "string.hpp"
#include "test_resource.hpp"
#include "inventory_manager.hpp"

using namespace server;
using namespace server_uts;

TEST(ut_inventory_manager, ctor_cctor_clone_dtor_id_sanity) {
	// GIVEN
	using TestManager = InventoryManager<std::string>;

	auto test_creator = [](const Object& data) {
		return new std::string(Data::cast<String>(data.access("data")).get());
	};
	auto test_copier = [](const std::string& data) {
		return new std::string(data);
	};
	auto test_reader = [](const std::string& instance) {
		Object data;
		data.add("data", String(instance));
		return data;
	};

	// WHEN
	using TestManagerUnqPtr = std::unique_ptr<TestManager>;
	using ResourceUnqPtr = std::unique_ptr<Resource>;
	TestManagerUnqPtr instance_ptr(nullptr);
	TestManagerUnqPtr instance_ptr_copy(nullptr);
	ResourceUnqPtr instance_ptr_clone(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = TestManagerUnqPtr(new TestManager(test_creator, test_copier, test_reader)));
	ASSERT_NE(nullptr, instance_ptr);

	// cctor
	ASSERT_NO_THROW(instance_ptr_copy = TestManagerUnqPtr(new TestManager(*instance_ptr)));
	ASSERT_NE(nullptr, instance_ptr_copy);

	// clone
	ASSERT_NO_THROW(instance_ptr_clone = ResourceUnqPtr(instance_ptr->clone()));
	ASSERT_NE(nullptr, instance_ptr_clone);

	// dtors
	ASSERT_NO_THROW(instance_ptr = nullptr);
	ASSERT_NO_THROW(instance_ptr_copy = nullptr);
	ASSERT_NO_THROW(instance_ptr_clone = nullptr);
}

// TEST(ut_inventory_manager, register_resource_contains_resource_sanity) {
// 	// GIVEN
// 	const std::string test_inventory_manager_id("test_inventory_manager_id");
// 	const std::string test_resource_id_1("test_resource_id_1");
// 	const std::string test_resource_id_2("test_resource_id_2");
// 	auto stub = [](const Request&) -> Response {
// 		throw std::runtime_error("NOT_IMPLEMENTED");
// 	};
// 	const TestResource test_resource_1(stub);
// 	const TestResource test_resource_2(stub);
	
// 	// WHEN
// 	TestManager instance;

// 	// THEN
// 	ASSERT_FALSE(instance.contains_resource(test_resource_id_1));
// 	ASSERT_NO_THROW(instance.register_resource(test_resource_id_1, test_resource_1));
// 	ASSERT_TRUE(instance.contains_resource(test_resource_id_1));

// 	ASSERT_FALSE(instance.contains_resource(test_resource_id_2));
// 	ASSERT_NO_THROW(instance.register_resource(test_resource_id_2, test_resource_2));
// 	ASSERT_TRUE(instance.contains_resource(test_resource_id_2));
// }

// TEST(ut_inventory_manager, run_request_sanity) {
// 	// GIVEN
// 	const std::string test_inventory_manager_id("test_inventory_manager_id");
	
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
// 	TestManager instance;
// 	instance.register_resource(test_resource_id_1, test_resource_1);
// 	instance.register_resource(test_resource_id_2, test_resource_2);
// 	std::unique_ptr<Response> response(nullptr);

// 	// THEN
// 	ASSERT_NO_THROW(response = std::make_unique<Response>(instance.run_request(test_request_1)));
// 	ASSERT_EQ(test_response_code_1, response->code());
// 	ASSERT_NO_THROW(response = std::make_unique<Response>(instance.run_request(test_request_2)));
// 	ASSERT_EQ(test_response_code_2, response->code());
// }