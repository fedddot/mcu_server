#ifndef	RESOURCES_VENDOR_HPP
#define	RESOURCES_VENDOR_HPP

#include <exception>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "manager.hpp"
#include "object.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "string.hpp"
#include "vendor.hpp"

namespace vendor {
	class ResourcesVendor: public server::Vendor {
	public:
		class ClonableManager: public server::Manager {
		public:
			virtual server::Manager *clone() const = 0;
		};
		
		ResourcesVendor() = default;
		ResourcesVendor(const ResourcesVendor& other) = delete;
		ResourcesVendor& operator=(const ResourcesVendor&) = delete;
		
		server::Response run_request(const server::Request& request) const override;
		
		void add_manager(const server::ResourceId& id, const ClonableManager& manager);
	private:
		std::map<server::ResourceId, std::unique_ptr<server::Manager>> m_managers;
			
		server::Response report_managers() const;
		server::Response run_create(server::Manager *manager, const server::Body& request_body) const;
		server::Response run_read(server::Manager *manager, const server::Path& route) const;
		server::Response run_update(server::Manager *manager, const server::Path& route, const server::Body& request_body) const;
		server::Response run_delete(server::Manager *manager, const server::Path& route) const;

		static server::ResourceId retrieve_manager_id_from_path(const server::Path& route);
	};
	
	inline server::ResourceId ResourcesVendor::retrieve_manager_id_from_path(const server::Path& route) {
		using namespace server;
		if (1UL > route.size()) {
			throw ServerException(ResponseCode::NOT_FOUND, "failed to retrieve manager id from path");
		}
		return route[0];
	}

	inline server::Response ResourcesVendor::run_request(const server::Request& request) const {
		using namespace server;
		if (Request::Method::READ == request.method() && request.path().empty()) {
			return report_managers();
		}
		const auto manager_id(retrieve_manager_id_from_path(request.path()));
		const auto iter = m_managers.find(manager_id);
		if (m_managers.end() == iter) {
			throw ServerException(ResponseCode::NOT_FOUND, "manager " + manager_id + " not registered");
		}
		auto manager_ptr((iter->second).get());
		const auto vendor_route(request.path());
		const auto manager_route(Path(vendor_route.begin() + 1, vendor_route.end()));

		switch (request.method()) {
		case Request::Method::CREATE:
			return run_create(manager_ptr, request.body());
		case Request::Method::READ:
			return run_read(manager_ptr, manager_route);
		case Request::Method::UPDATE:
			return run_update(manager_ptr, manager_route, request.body());
		case Request::Method::DELETE:
			return run_delete(manager_ptr, manager_route);
		default:
			throw ServerException(ResponseCode::METHOD_NOT_ALLOWED, "unsupported method received");
		}
	}
	
	inline void ResourcesVendor::add_manager(const server::ResourceId& id, const ClonableManager& manager) {
		if (m_managers.end() != m_managers.find(id)) {
			throw std::invalid_argument("manager already exists");
		}
		m_managers.insert({id, std::unique_ptr<server::Manager>(manager.clone())});
	}

	inline server::Response ResourcesVendor::report_managers() const {
		using namespace server;
		Object managers;
		for (const auto& [id, manager_ptr]: m_managers) {
			managers.add(id, manager_ptr->read_all_resources());
		}
		return Response(ResponseCode::OK, Body(managers));
	}
	
	inline server::Response ResourcesVendor::run_create(server::Manager *manager, const server::Body& request_body) const {
		using namespace server;
		ResourceId resource_id("");
		Object create_cfg;
		try {
			resource_id = static_cast<ResourceId>(Data::cast<String>(request_body.access("id")).get());
			create_cfg = Data::cast<Object>(request_body.access("config"));
		} catch (const std::exception& e) {
			throw ServerException(ResponseCode::BAD_REQUEST, "failed to retrieve resource id or config fields crom create request: " + std::string(e.what()));
		}
		manager->create_resource(resource_id, create_cfg);
		return Response(ResponseCode::OK, Body());
	}

	inline server::Response ResourcesVendor::run_read(server::Manager *manager, const server::Path& route) const {
		using namespace server;
		return Response(ResponseCode::OK, Body(manager->read_resource(route)));
	}
	
	inline server::Response ResourcesVendor::run_update(server::Manager *manager, const server::Path& route, const server::Body& request_body) const {
		using namespace server;
		manager->update_resource(route, request_body);
		return Response(ResponseCode::OK, Body());
	}
	
	inline server::Response ResourcesVendor::run_delete(server::Manager *manager, const server::Path& route) const {
		using namespace server;
		manager->delete_resource(route);
		return Response(ResponseCode::OK, Body());
	}
}

#endif // RESOURCES_VENDOR_HPP