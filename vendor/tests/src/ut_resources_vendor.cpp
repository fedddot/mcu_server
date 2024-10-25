#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "in_memory_inventory.hpp"
#include "inventory_manager.hpp"
#include "manager.hpp"
#include "object.hpp"
#include "request.hpp"
#include "resources_vendor.hpp"
#include "response.hpp"
#include "server_types.hpp"
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

class TestClonableManager: public ResourcesVendor::ClonableManager {
public:
	TestClonableManager(Manager *manager): m_manager(manager) {

	}
	TestClonableManager(const TestClonableManager& other) = default;
	TestClonableManager& operator=(const TestClonableManager& other) = delete;
	void create_resource(const ResourceId& id, const Data& create_cfg) override {
		m_manager->create_resource(id, create_cfg);
	}
	Object read_resource(const Path& route) const override {
		return m_manager->read_resource(route);
	}
	Object read_all_resources() const override {
		return m_manager->read_all_resources();
	}
	void update_resource(const Path& route, const Data& update_cfg) override {
		m_manager->update_resource(route, update_cfg);
	}
	void delete_resource(const Path& route) override {
		m_manager->delete_resource(route);
	}
	bool contains(const Path& route) const override {
		return m_manager->contains(route);
	}
	server::Manager *clone() const override {
		return new TestClonableManager(*this);
	}
private:
	std::shared_ptr<Manager> m_manager;
};

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
		TestClonableManager(
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