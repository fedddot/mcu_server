#ifndef	RESOURCES_VENDOR_HPP
#define	RESOURCES_VENDOR_HPP

#include <stdexcept>
#include <string>

#include "manager.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
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
		
		class VendorException: public server::ServerException {
		public:
			VendorException(const server::ResponseCode& code, const std::string& what);
			server::Body body() const override;
			server::ResponseCode code() const override;
			const char *what() const noexcept override;
		};
		
		server::Response report_managers() const;
		server::Response run_create(server::Manager *manager, const server::Body& request_body) const;
		server::Response run_read(server::Manager *manager, const server::Path& route) const;
		server::Response run_update(server::Manager *manager, const server::Path& route, const server::Body& request_body) const;
		server::Response run_delete(server::Manager *manager, const server::Path& route) const;
	};
	
	inline server::Response ResourcesVendor::run_request(const server::Request& request) const {
		using namespace server;
		if (Request::Method::READ == request.method() && request.path().empty()) {
			return report_managers();
		}
		const auto manager_id(request.path()[0]);
		const auto iter = m_managers.find(manager_id);
		if (m_managers.end() == iter) {
			throw VendorException(ResponseCode::NOT_FOUND, "manager not found");
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
			throw VendorException(ResponseCode::METHOD_NOT_ALLOWED, "unsupported method received");
		}
	}
	
	inline void ResourcesVendor::add_manager(const server::ResourceId& id, const ClonableManager& manager) {
		if (m_managers.end() != m_managers.find(id)) {
			throw std::invalid_argument("manager already exists");
		}
		m_managers.insert({id, std::unique_ptr<server::Manager>(manager.clone())});
	}

	inline server::Response ResourcesVendor::report_managers() const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
	
	inline server::Response ResourcesVendor::run_create(server::Manager *manager, const server::Body& request_body) const {
		using namespace server;
		manager->create_resource(request_body);
		return Response(ResponseCode::OK, Body());
	}

	inline server::Response ResourcesVendor::run_read(server::Manager *manager, const server::Path& route) const {
		using namespace server;
		return Response(ResponseCode::OK, manager->read_resource(route));
	}
	
	inline server::Response ResourcesVendor::run_update(server::Manager *manager, const server::Request& request) const {
		using namespace server;
		const auto resource_id(m_resource_id_retriever(request));
		if (!(manager->contains(resource_id))) {
			return report_failure(Response::ResponseCode::NOT_FOUND, "resource not found");
		}
		const auto update_cfg(m_update_cfg_retriever(request));
		manager->update_resource(resource_id, update_cfg);
		return Response(Response::ResponseCode::OK, Response::Body());
	}

	
	inline server::Response ResourcesVendor::run_delete(server::Manager *manager, const server::Request& request) const {
		using namespace server;
		const auto resource_id(m_resource_id_retriever(request));
		if (!(manager->contains(resource_id))) {
			return report_failure(Response::ResponseCode::NOT_FOUND, "resource not found");
		}
		manager->delete_resource(resource_id);
		return Response(Response::ResponseCode::OK, Response::Body());
	}
}

#endif // RESOURCES_VENDOR_HPP