#include <map>
#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "manager.hpp"
#include "object.hpp"
#include "request.hpp"
#include "resources_vendor.hpp"
#include "response.hpp"
#include "string.hpp"

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

class TestManager: public ResourcesVendor<ResourceId>::ClonableManager {
public:
	TestManager() = default;
	TestManager(const TestManager&) = default;
	void create_resource(const ResourceId& id, const Object& create_config) override {
		if (contains(id)) {
			throw std::invalid_argument("already created");
		}
		m_resources_data.insert({id, create_config});
	}

	Object read_resource(const ResourceId& id) const override {
		if (!contains(id)) {
			throw std::invalid_argument("not found");
		}
		return m_resources_data.at(id);
	}

	Object read_all_resources() const override {
		Object all_resources;
		for (const auto& [key, val]: m_resources_data) {
			all_resources.add(key, val);
		}
		return all_resources;
	}

	void update_resource(const ResourceId& id, const Object& update_config) override {
		if (!contains(id)) {
			throw std::invalid_argument("not found");
		}
		m_resources_data[id] = update_config;
	}
	
	void delete_resource(const ResourceId& id) override {
		if (!contains(id)) {
			throw std::invalid_argument("not found");
		}
		auto iter = m_resources_data.find(id);
		m_resources_data.erase(iter);
	}

	bool contains(const ResourceId& id) const override {
		return m_resources_data.end() != m_resources_data.find(id);
	}

	Manager<ResourceId> *clone() const override {
		return new TestManager(*this);
	}
private:
	std::map<ResourceId, Object> m_resources_data;
};

TEST(ut_resources_vendor, run_request_sanity) {
	// GIVEN
	using Path = typename Request::Path;
	using Method = typename Request::Method;
	using RequestBody = typename Request::Body;
	
	using ResponseCode = typename Response::ResponseCode;
	using ResponseBody = typename Response::Body;

	const ResourceId manager_id("test_manager");
	const ResourceId resourse_id("test_resource");

	const Path create_path { manager_id };
	Object create_cfg;
	create_cfg.add("param", String("val"));
	RequestBody create_body;
	create_body.add("id", String(resourse_id));
	create_body.add("config", create_cfg);
	const Request create_request(Method::CREATE, create_path, create_body);

	const Path read_path {manager_id, resourse_id};
	const Request read_request(Method::READ, read_path, RequestBody());

	const Path update_path { manager_id, resourse_id };
	Object update_cfg;
	update_cfg.add("param", String("valval"));
	RequestBody update_body;
	update_body.add("config", update_cfg);
	const Request update_request(Method::UPDATE, update_path, update_body);

	const Path delete_path {manager_id, resourse_id};
	const Request delete_request(Method::DELETE, read_path, RequestBody());


	// WHEN
	ResourcesVendor<ResourceId> instance(
		retrieve_manager_id,
		retrieve_resource_id,
		retrieve_create_data,
		retrieve_update_data
	);
	instance.add_manager(manager_id, TestManager());
	Response response(ResponseCode::UNSPECIFIED, ResponseBody());

	// THEN
	ASSERT_NO_THROW(response = instance.run_request(create_request));
	ASSERT_EQ(ResponseCode::OK, response.code());

	ASSERT_NO_THROW(response = instance.run_request(read_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("param"));
	ASSERT_EQ(Data::cast<String>(create_cfg.access("param")).get(), Data::cast<String>(response.body().access("param")).get());

	ASSERT_NO_THROW(response = instance.run_request(update_request));
	ASSERT_EQ(ResponseCode::OK, response.code());

	ASSERT_NO_THROW(response = instance.run_request(read_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
	ASSERT_TRUE(response.body().contains("param"));
	ASSERT_EQ(Data::cast<String>(update_cfg.access("param")).get(), Data::cast<String>(response.body().access("param")).get());

	ASSERT_NO_THROW(response = instance.run_request(delete_request));
	ASSERT_EQ(ResponseCode::OK, response.code());
}