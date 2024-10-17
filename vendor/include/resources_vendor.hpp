#ifndef	RESOURCES_VENDOR_HPP
#define	RESOURCES_VENDOR_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <map>

#include "manager.hpp"
#include "object.hpp"
#include "request.hpp"
#include "response.hpp"
#include "string.hpp"
#include "vendor.hpp"

namespace vendor {
	template <typename Tresource_id>
	class ResourcesVendor: public server::Vendor {
	public:
		class ClonableManager: public server::Manager<Tresource_id> {
		public:
			virtual server::Manager<Tresource_id> *clone() const = 0;
		};
		using IdRetriever = std::function<Tresource_id(const server::Request&)>;
		using ConfigRetriever = std::function<server::Object(const server::Request&)>;
		
		ResourcesVendor(
			const IdRetriever& manager_id_retriever,
			const IdRetriever& resource_id_retriever,
			const ConfigRetriever& create_cfg_retriever,
			const ConfigRetriever& update_cfg_retriever
		);
		ResourcesVendor(const ResourcesVendor& other) = delete;
		ResourcesVendor& operator=(const ResourcesVendor&) = delete;
		
		server::Response run_request(const server::Request& request) const override;
		
		void add_manager(const Tresource_id& id, const ClonableManager& manager);
	private:
		IdRetriever m_manager_id_retriever;
		IdRetriever m_resource_id_retriever;
		ConfigRetriever m_create_cfg_retriever;
		ConfigRetriever m_update_cfg_retriever;

		std::map<Tresource_id, std::unique_ptr<server::Manager<Tresource_id>>> m_managers;
		
		server::Response report_failure(const server::Response::ResponseCode& code, const std::string& what) const;
		
		server::Response report_managers() const;
		server::Response run_create(server::Manager<Tresource_id> *manager, const server::Request& request) const;
		server::Response run_read(server::Manager<Tresource_id> *manager, const server::Request& request) const;
		server::Response run_update(server::Manager<Tresource_id> *manager, const server::Request& request) const;
		server::Response run_delete(server::Manager<Tresource_id> *manager, const server::Request& request) const;
	};

	template <typename Tresource_id>
	inline ResourcesVendor<Tresource_id>::ResourcesVendor(
			const IdRetriever& manager_id_retriever,
			const IdRetriever& resource_id_retriever,
			const ConfigRetriever& create_cfg_retriever,
			const ConfigRetriever& update_cfg_retriever
	): m_manager_id_retriever(manager_id_retriever), m_resource_id_retriever(resource_id_retriever), m_create_cfg_retriever(create_cfg_retriever), m_update_cfg_retriever(update_cfg_retriever) {

	}

	template <typename Tresource_id>
	inline server::Response ResourcesVendor<Tresource_id>::run_request(const server::Request& request) const {
		using namespace server;
		if (Request::Method::READ == request.method() && request.path().empty()) {
			return report_managers();
		}
		const auto manager_id(m_manager_id_retriever(request));
		const auto iter = m_managers.find(manager_id);
		if (m_managers.end() == iter) {
			return report_failure(Response::ResponseCode::NOT_FOUND, "manager not found");
		}
		auto manager_ptr((iter->second).get());

		switch (request.method()) {
		case Request::Method::CREATE:
			return run_create(manager_ptr, request);
		case Request::Method::READ:
			return run_read(manager_ptr, request);
		case Request::Method::UPDATE:
			return run_update(manager_ptr, request);
		case Request::Method::DELETE:
			return run_delete(manager_ptr, request);
		default:
			return report_failure(Response::ResponseCode::METHOD_NOT_ALLOWED, "unsupported method received");
		}
	}

	template <typename Tresource_id>
	inline void ResourcesVendor<Tresource_id>::add_manager(const Tresource_id& id, const ClonableManager& manager) {
		if (m_managers.end() != m_managers.find(id)) {
			throw std::invalid_argument("manager already exists");
		}
		m_managers.insert({id, std::unique_ptr<server::Manager<Tresource_id>>(manager.clone())});
	}

	template <typename Tresource_id>
	inline server::Response ResourcesVendor<Tresource_id>::report_failure(const server::Response::ResponseCode& code, const std::string& what) const {
		server::Response::Body body;
		body.add("what", server::String(what));
		return server::Response(code, body);
	}
		
	template <typename Tresource_id>
	inline server::Response ResourcesVendor<Tresource_id>::report_managers() const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	template <typename Tresource_id>
	inline server::Response ResourcesVendor<Tresource_id>::run_create(server::Manager<Tresource_id> *manager, const server::Request& request) const {
		using namespace server;
		const auto resource_id(m_resource_id_retriever(request));
		if (manager->contains(resource_id)) {
			return report_failure(Response::ResponseCode::BAD_REQUEST, "resource is already created");
		}
		const auto create_cfg(m_create_cfg_retriever(request));
		manager->create_resource(resource_id, create_cfg);
		return Response(Response::ResponseCode::OK, Response::Body());
	}

	template <typename Tresource_id>
	inline server::Response ResourcesVendor<Tresource_id>::run_read(server::Manager<Tresource_id> *manager, const server::Request& request) const {
		using namespace server;
		const auto resource_id(m_resource_id_retriever(request));
		if (!(manager->contains(resource_id))) {
			return report_failure(Response::ResponseCode::NOT_FOUND, "resource not found");
		}
		const auto resource_data(manager->read_resource(resource_id));
		return Response(Response::ResponseCode::OK, Response::Body(resource_data));
	}

	template <typename Tresource_id>
	inline server::Response ResourcesVendor<Tresource_id>::run_update(server::Manager<Tresource_id> *manager, const server::Request& request) const {
		using namespace server;
		const auto resource_id(m_resource_id_retriever(request));
		if (!(manager->contains(resource_id))) {
			return report_failure(Response::ResponseCode::NOT_FOUND, "resource not found");
		}
		const auto update_cfg(m_update_cfg_retriever(request));
		manager->update_resource(resource_id, update_cfg);
		return Response(Response::ResponseCode::OK, Response::Body());
	}

	template <typename Tresource_id>
	inline server::Response ResourcesVendor<Tresource_id>::run_delete(server::Manager<Tresource_id> *manager, const server::Request& request) const {
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