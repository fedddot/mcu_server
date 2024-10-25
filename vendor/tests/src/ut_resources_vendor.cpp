#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "in_memory_inventory.hpp"
#include "inventory_manager.hpp"
#include "object.hpp"
#include "request.hpp"
#include "resources_vendor.hpp"
#include "response.hpp"
#include "server_types.hpp"
#include "clonable_manager_wrapper.hpp"
#include "string.hpp"

using namespace server;
using namespace vendor;
using namespace manager;

TEST(ut_resources_vendor, ctor_dtor_id_sanity) {
	// WHEN
	std::unique_ptr<ResourcesVendor> instance_ptr(nullptr);
	
	// THEN
	// ctor
	ASSERT_NO_THROW(instance_ptr = std::unique_ptr<ResourcesVendor>(new ResourcesVendor()));
	ASSERT_NE(nullptr, instance_ptr);

	// dtor
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_resources_vendor, run_request_sanity) {
	// GIVEN
	const ResourceId manager_id("test_manager");
	const ResourceId resourse_id("test_resource");
	const std::string init_val("init_val");
	const std::string updated_val("updated_val");

	const Path create_path { manager_id };
	Object create_cfg;
	create_cfg.add("val", String(init_val));
	Body create_body;
	create_body.add("id", String(resourse_id));
	create_body.add("config", create_cfg);
	const Request create_request(Request::Method::CREATE, create_path, create_body);

	const Path read_path {manager_id, resourse_id};
	const Request read_request(Request::Method::READ, read_path, Body());

	const Path update_path { manager_id, resourse_id };
	Object update_cfg;
	update_cfg.add("val", String(updated_val));
	const Request update_request(Request::Method::UPDATE, update_path, update_cfg);

	const Path delete_path {manager_id, resourse_id};
	const Request delete_request(Request::Method::DELETE, read_path, Body());


	// WHEN
	InMemoryInventory<ResourceId, std::string> inventory;
	ResourcesVendor instance;
	instance.add_manager(
		manager_id,
		ClonableManagerWrapper(
			new InventoryManager<std::string>(
				&inventory,
				[](const Data& cfg) {
					const auto init_val = Data::cast<String>(Data::cast<Object>(cfg).access("val")).get();
					return new std::string(init_val);
				},
				[](const std::string& item) {
					Object item_data;
					item_data.add("val", String(item));
					return item_data;
				},
				[](std::string *item, const Data& cfg) {
					*item = Data::cast<String>(Data::cast<Object>(cfg).access("val")).get();
				}
			)
		)
	);
	Response response(ResponseCode::UNSPECIFIED, Body());

	// THEN
	ASSERT_NO_THROW(response = instance.run_request(create_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(inventory.contains(resourse_id));

	ASSERT_NO_THROW(response = instance.run_request(read_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("val"));
	ASSERT_EQ(init_val, Data::cast<String>(response.body().access("val")).get());

	ASSERT_NO_THROW(response = instance.run_request(update_request));
	ASSERT_EQ(ResponseCode::OK, response.code());

	ASSERT_NO_THROW(response = instance.run_request(read_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("val"));
	ASSERT_EQ(updated_val, Data::cast<String>(response.body().access("val")).get());

	ASSERT_NO_THROW(response = instance.run_request(delete_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_FALSE(inventory.contains(resourse_id));
}