#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "gtest/gtest.h"

#include "array.hpp"
#include "data.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
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

TEST(ut_gpio_manager, run_request_sanity) {
	// GIVEN
	InMemoryInventory<TestId, Gpio> inventory;
	auto gpio_ctor = [](const Request::Body& body) -> Gpio * {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	auto id_from_body_retriever = [](const Request::Body& body) -> TestId {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	auto id_from_path_retriever = [](const Request::Path& path) -> TestId {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	auto gpio_reader = [](const Gpio&) -> Response::Body {
		throw std::runtime_error("NOT IMPLEMENTED");
	};
	auto gpio_writer = [](Gpio *, const Request::Body&) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	const TestId test_id(10);
	Object gpio_config;
	gpio_config.add("dir", Integer(static_cast<int>(Gpio::Direction::OUT)));
	gpio_config.add("pin_number", Integer(test_id));
	Request::Body create_body;
	create_body.add("id", Integer(test_id));
	create_body.add("config", gpio_config);
	const Request create_request(Request::Method::CREATE, {}, create_body);

	// WHEN
	TestVendor vendor(
		[test_config, data_to_method, data_to_path](const Gpio& request) -> Response {
			if (request.method() != data_to_method(test_config.access("method"))) {
				throw std::runtime_error("");
			}
			if (request.path() != data_to_path(test_config.access("path"))) {
				throw std::runtime_error("");
			}
			return Response(Response::ResponseCode::OK, Response::Body());
		}
	);

	GpioManager<TestId> instance(
		&inventory,
		&vendor,
		request_retriever,
		id_from_body_retriever,
		id_from_path_retriever,
		ids_retriever,
		request_reader
	);
	Response response(Response::ResponseCode::UNSPECIFIED, Response::Body());

	// THEN
	ASSERT_NO_THROW(response = instance.run_request(test_request));
	ASSERT_EQ(Response::ResponseCode::OK, response.code());
	ASSERT_NO_THROW(response = instance.run_request(update_request));
	ASSERT_EQ(Response::ResponseCode::OK, response.code());
	ASSERT_NO_THROW(response = instance.run_request(delete_request));
	ASSERT_EQ(Response::ResponseCode::OK, response.code());
}