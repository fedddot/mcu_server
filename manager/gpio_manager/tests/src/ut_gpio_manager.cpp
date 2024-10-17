#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "gtest/gtest.h"

#include "array.hpp"
#include "data.hpp"
#include "in_memory_inventory.hpp"
#include "object.hpp"
#include "gpio_manager.hpp"
#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

using namespace server;
using namespace manager;

using TestId = int;

TEST(ut_gpio_manager, ctor_cctor_clone_dtor_id_sanity) {
	// GIVEN
	InMemoryInventory<TestId, Gpio> inventory;

	// WHEN
	using GpioManagerUnqPtr = std::unique_ptr<GpioManager<TestId>>;
	using ResourceUnqPtr = std::unique_ptr<Resource>;
	GpioManagerUnqPtr instance_ptr(nullptr);
	GpioManagerUnqPtr instance_ptr_copy(nullptr);
	ResourceUnqPtr instance_ptr_clone(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(
		instance_ptr = GpioManagerUnqPtr(
			new GpioManager<TestId>(
				&inventory,
				[](const Request::Body& body) -> Gpio * {
					throw std::runtime_error("NOT IMPLEMENTED");
				},
				[](const Request::Body& body) -> TestId {
					throw std::runtime_error("NOT IMPLEMENTED");
				},
				[](const Request::Path& path) -> TestId {
					throw std::runtime_error("NOT IMPLEMENTED");
				},
				[](const Gpio&) -> Response::Body {
					throw std::runtime_error("NOT IMPLEMENTED");
				},
				[](Gpio *, const Request::Body&) {
					throw std::runtime_error("NOT IMPLEMENTED");
				}
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);

	// cctor
	ASSERT_NO_THROW(instance_ptr_copy = GpioManagerUnqPtr(new GpioManager(*instance_ptr)));
	ASSERT_NE(nullptr, instance_ptr_copy);

	// clone
	ASSERT_NO_THROW(instance_ptr_clone = ResourceUnqPtr(instance_ptr->clone()));
	ASSERT_NE(nullptr, instance_ptr_clone);

	// dtors
	ASSERT_NO_THROW(instance_ptr = nullptr);
	ASSERT_NO_THROW(instance_ptr_copy = nullptr);
	ASSERT_NO_THROW(instance_ptr_clone = nullptr);
}

// TEST(ut_gpio_manager, run_request_sanity) {
// 	// GIVEN
// 	InMemoryInventory<TestId, Gpio> inventory;
// 	auto data_to_method = [](const Data& data) {
// 		static const std::map<std::string, Gpio::Method> mapping {
// 			{"CREATE", Gpio::Method::CREATE},
// 			{"READ", Gpio::Method::READ},
// 			{"UPDATE", Gpio::Method::UPDATE},
// 			{"DELETE", Gpio::Method::DELETE}
// 		};
// 		const auto iter = mapping.find(Data::cast<String>(data).get());
// 		if (mapping.end() == iter) {
// 			throw std::invalid_argument("unsupported method received");
// 		}
// 		return iter->second;
// 	};
// 	auto data_to_path = [](const Data& data) {
// 		Request::Path path;
// 		Data::cast<Array>(data).for_each(
// 			[&path](int, const Data& data) {
// 				path.push_back(Data::cast<String>(data).get());
// 			}
// 		);
// 		return path;
// 	};
// 	auto request_retriever = [data_to_method, data_to_path](const Request::Body& body) {
// 		auto method = data_to_method(Data::cast<Object>(body.access("config")).access("method"));
// 		auto path = data_to_path(Data::cast<Object>(body.access("config")).access("path"));
// 		return Gpio(method, path, Data::cast<Request::Body>(Data::cast<Object>(body.access("config")).access("body")));
// 	};
// 	auto id_from_body_retriever = [](const Request::Body& body) {
// 		return static_cast<TestId>(Data::cast<String>(body.access("id")).get());
// 	};
// 	auto id_from_path_retriever = [](const Request::Path& path) {
// 		if (path.empty()) {
// 			throw std::invalid_argument("empty path received");
// 		}
// 		return static_cast<TestId>(path[0]);
// 	};
// 	auto ids_retriever = [](const Request::Body& body) {
// 		const auto& ids_array(Data::cast<Array>(body.access("ids"))); 
// 		std::vector<TestId> ids;
// 		ids.reserve(ids_array.size());
// 		ids_array.for_each(
// 			[&ids](int, const Data& data) {
// 				ids.push_back(static_cast<TestId>(Data::cast<String>(data).get()));
// 			}
// 		);
// 		return ids;
// 	};
// 	auto request_reader = [](const Gpio&) -> Response::Body {
// 		throw std::runtime_error("NOT IMPLEMENTED");
// 	};

// 	const TestId test_id("test_request");
// 	Array request_path;
// 	request_path.push_back(String("gpiod"));
// 	Object test_config;
// 	test_config.add("method", String("CREATE"));
// 	test_config.add("path", request_path);
// 	test_config.add("body", Request::Body());
// 	Request::Body create_body;
// 	create_body.add("id", String(test_id));
// 	create_body.add("config", test_config);
// 	Gpio test_request(Gpio::Method::CREATE, {}, create_body);

// 	Request::Body update_body;
// 	Array ids_array;
// 	ids_array.push_back(String(test_id));
// 	update_body.add("ids", ids_array);
// 	Gpio update_request(Gpio::Method::UPDATE, {}, update_body);
// 	Gpio delete_request(Gpio::Method::DELETE, {test_id}, Request::Body());

// 	// WHEN
// 	TestVendor vendor(
// 		[test_config, data_to_method, data_to_path](const Gpio& request) -> Response {
// 			if (request.method() != data_to_method(test_config.access("method"))) {
// 				throw std::runtime_error("");
// 			}
// 			if (request.path() != data_to_path(test_config.access("path"))) {
// 				throw std::runtime_error("");
// 			}
// 			return Response(Response::ResponseCode::OK, Response::Body());
// 		}
// 	);

// 	GpioManager<TestId> instance(
// 		&inventory,
// 		&vendor,
// 		request_retriever,
// 		id_from_body_retriever,
// 		id_from_path_retriever,
// 		ids_retriever,
// 		request_reader
// 	);
// 	Response response(Response::ResponseCode::UNSPECIFIED, Response::Body());

// 	// THEN
// 	ASSERT_NO_THROW(response = instance.run_request(test_request));
// 	ASSERT_EQ(Response::ResponseCode::OK, response.code());
// 	ASSERT_NO_THROW(response = instance.run_request(update_request));
// 	ASSERT_EQ(Response::ResponseCode::OK, response.code());
// 	ASSERT_NO_THROW(response = instance.run_request(delete_request));
// 	ASSERT_EQ(Response::ResponseCode::OK, response.code());
// }