#include <map>
#include <memory>

#include "gtest/gtest.h"

#include "manager.hpp"
#include "object.hpp"
#include "request.hpp"
#include "resources_vendor.hpp"
#include "response.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "string.hpp"

using namespace server;
using namespace vendor;

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

class TestManager: public ResourcesVendor::ClonableManager {
public:
	TestManager() = default;
	TestManager(const TestManager&) = default;
	void create_resource(const Body& create_cfg) override {
		const auto id(Data::cast<String>(create_cfg.access("id")).get());
		if (contains({id})) {
			throw ServerException(ResponseCode::BAD_REQUEST, "resourse already exists");
		}
		m_resources_data.insert({id, create_cfg});
	}

	Body read_resource(const Path& route) const override {
		if (!contains(route)) {
			throw ServerException(ResponseCode::NOT_FOUND, "not found");
		}
		return m_resources_data.at(route[0]);
	}

	Object read_all_resources() const override {
		Object all_resources;
		for (const auto& [key, val]: m_resources_data) {
			all_resources.add(key, val);
		}
		return all_resources;
	}

	void update_resource(const Path& route, const Body& update_config) override {
		if (!contains(route)) {
			throw ServerException(ResponseCode::NOT_FOUND, "not found");
		}
		m_resources_data[route[0]] = update_config;
	}
	
	void delete_resource(const Path& route) override {
		if (!contains(route)) {
			throw ServerException(ResponseCode::NOT_FOUND, "not found");
		}
		auto iter = m_resources_data.find(route[0]);
		m_resources_data.erase(iter);
	}

	bool contains(const Path& route) const override {
		return m_resources_data.end() != m_resources_data.find(route[0]);
	}

	Manager *clone() const override {
		return new TestManager(*this);
	}
private:
	std::map<ResourceId, Body> m_resources_data;
};

TEST(ut_resources_vendor, run_request_sanity) {
	// GIVEN
	const ResourceId manager_id("test_manager");
	const ResourceId resourse_id("test_resource");

	const Path create_path { manager_id };
	Object create_cfg;
	create_cfg.add("param", String("val"));
	Body create_body;
	create_body.add("id", String(resourse_id));
	create_body.add("config", create_cfg);
	const Request create_request(Request::Method::CREATE, create_path, create_body);

	const Path read_path {manager_id, resourse_id};
	const Request read_request(Request::Method::READ, read_path, Body());

	const Path update_path { manager_id, resourse_id };
	Object update_cfg;
	update_cfg.add("param", String("valval"));
	Body update_body;
	update_body.add("config", update_cfg);
	const Request update_request(Request::Method::UPDATE, update_path, update_body);

	const Path delete_path {manager_id, resourse_id};
	const Request delete_request(Request::Method::DELETE, read_path, Body());


	// WHEN
	ResourcesVendor instance;
	instance.add_manager(manager_id, TestManager());
	Response response(ResponseCode::UNSPECIFIED, Body());

	// THEN
	ASSERT_NO_THROW(response = instance.run_request(create_request));
	ASSERT_EQ(ResponseCode::OK, response.code());

	ASSERT_NO_THROW(response = instance.run_request(read_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("config"));

	ASSERT_NO_THROW(response = instance.run_request(update_request));
	ASSERT_EQ(ResponseCode::OK, response.code());

	ASSERT_NO_THROW(response = instance.run_request(read_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("config"));

	ASSERT_NO_THROW(response = instance.run_request(delete_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
}