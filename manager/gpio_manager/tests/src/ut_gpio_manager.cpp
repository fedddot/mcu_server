#include "gtest/gtest.h"
#include <memory>
#include <stdexcept>
#include "data.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "request.hpp"
#include "response.hpp"

using namespace server;
using namespace manager;

using Method = typename Request::Method;
using Path = typename Request::Path;
using Body = typename Request::Body;
using ResponseCode = typename Response::ResponseCode;	

TEST(ut_gpio_manager, ctor_cctor_clone_dtor_id_sanity) {
	// GIVEN
	auto test_creator = [](const Data& data)-> Gpio * {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	using GpioManagerUnqPtr = std::unique_ptr<GpioManager>;
	using ResourceUnqPtr = std::unique_ptr<Resource>;
	GpioManagerUnqPtr instance_ptr(nullptr);
	GpioManagerUnqPtr instance_ptr_copy(nullptr);
	ResourceUnqPtr instance_ptr_clone(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = GpioManagerUnqPtr(new GpioManager(test_creator)));
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
// 	using GpioManager = GpioManager;
	
// 	auto test_creator = [](const Data& data) {
// 		return new std::string(Data::cast<String>(Data::cast<Object>(data).access("data")).get());
// 	};
// 	auto test_copier = [](const std::string& data) {
// 		return new std::string(data);
// 	};
// 	auto test_reader = [](const std::string& instance) {
// 		Object data;
// 		data.add("data", String(instance));
// 		return data;
// 	};
// 	auto test_writer = [](std::string *instance_ptr, const Data& data) {
// 		*instance_ptr = Data::cast<String>(Data::cast<Object>(data).access("data")).get();
// 	};

// 	Body create_instance_data;
// 	create_instance_data.add("id", String("instance_id"));
// 	create_instance_data.add("data", String("instance_data"));

// 	Body update_instance_data;
// 	update_instance_data.add("data", String("updated_instance_data"));
	
// 	// WHEN
// 	GpioManager instance(test_creator, test_copier, test_reader, test_writer);
// 	Response response(ResponseCode::OK, Body());

// 	// THEN
// 	// Create
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::CREATE, 
// 				{}, 
// 				create_instance_data
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
	
// 	// ReadAll
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::READ, 
// 				{}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// 	ASSERT_TRUE(response.body().contains("members"));
// 	ASSERT_EQ(Data::Type::ARRAY, response.body().access("members").type());
// 	ASSERT_EQ(1UL, Data::cast<Array>(response.body().access("members")).size());

// 	// Read
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::READ, 
// 				{Data::cast<String>(create_instance_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// 	ASSERT_TRUE(response.body().contains("id"));
// 	ASSERT_EQ(Data::cast<String>(create_instance_data.access("id")).get(), Data::cast<String>(response.body().access("id")).get());
	
// 	ASSERT_TRUE(response.body().contains("data"));
// 	ASSERT_EQ(Data::cast<String>(create_instance_data.access("data")).get(), Data::cast<String>(response.body().access("data")).get());

// 	// Update
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::UPDATE, 
// 				{Data::cast<String>(create_instance_data.access("id")).get()}, 
// 				update_instance_data
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// 	// Update validation
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::READ, 
// 				{Data::cast<String>(create_instance_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
// 	ASSERT_TRUE(response.body().contains("id"));
// 	ASSERT_EQ(Data::cast<String>(create_instance_data.access("id")).get(), Data::cast<String>(response.body().access("id")).get());
	
// 	ASSERT_TRUE(response.body().contains("data"));
// 	ASSERT_EQ(Data::cast<String>(update_instance_data.access("data")).get(), Data::cast<String>(response.body().access("data")).get());

// 	// Delete
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::DELETE, 
// 				{Data::cast<String>(create_instance_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_EQ(ResponseCode::OK, response.code());
	
// 	// Delete validation
// 	ASSERT_NO_THROW(
// 		response = instance.run_request(
// 			Request(
// 				Method::READ, 
// 				{Data::cast<String>(create_instance_data.access("id")).get()}, 
// 				Body()
// 			)
// 		)
// 	);
// 	ASSERT_NE(ResponseCode::OK, response.code());
// }