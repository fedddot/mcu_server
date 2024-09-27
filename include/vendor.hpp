#ifndef	VENDOR_HPP
#define	VENDOR_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace server {
	class Vendor: public Resource {
	public:
		Vendor(const std::string& id);
		Vendor(const Vendor&);
		Vendor& operator=(const Vendor&) = delete;
		
		std::string id() const override;
		Response run_request(const Request&) const override;
		Resource *clone() const override;
		
		void register_resource(const Resource& resource);
		bool contains_resource(const std::string& id) const;
	private:
		std::string m_id;
		std::vector<std::unique_ptr<Resource>> m_resources;

		Resource *access_resource(const std::string& id) const;

		Response run_create_request(const Request& request) const;
		Response run_read_request(const Request& request) const;
		Response run_update_request(const Request& request) const;
		Response run_delete_request(const Request& request) const;
	};

	inline Vendor::Vendor(const std::string& id): m_id(id) {

	}

	inline Vendor::Vendor(const Vendor& other): m_id(other.m_id) {
		for (const auto& resource: other.m_resources) {
			m_resources.push_back(std::unique_ptr<Resource>(resource->clone()));
		}
	}

	inline std::string Vendor::id() const {
		return m_id;
	}

	inline Response Vendor::run_request(const Request& request) const {
		switch (request.method()) {
		case Request::Method::CREATE:
			return run_create_request(request);
		case Request::Method::READ:
			return run_read_request(request);
		case Request::Method::UPDATE:
			return run_update_request(request);
		case Request::Method::DELETE:
			return run_delete_request(request);
		default:
			break;
		}
		Response::Body response_data;
		response_data.add("what", server::String("received method is not supported"));
		return Response(Response::ResponseCode::METHOD_NOT_ALLOWED, response_data);
	}

	inline Resource *Vendor::clone() const {
		return new Vendor(*this);
	}

	inline void Vendor::register_resource(const Resource& resource) {
		if (contains_resource(resource.id())) {
			throw std::invalid_argument("resource id " + resource.id() + " is already registered");
		}
		m_resources.push_back(std::unique_ptr<Resource>(resource.clone()));
	}

	inline bool Vendor::contains_resource(const std::string& id) const {
		for (auto const& registered_resource: m_resources) {
			if (registered_resource->id() == id) {
				return true;
			}
		}
		return false;
	}

	inline Resource *Vendor::access_resource(const std::string& id) const {
		for (auto const& registered_resource: m_resources) {
			if (registered_resource->id() == id) {
				registered_resource.get();
			}
		}
		throw std::invalid_argument("resource " + id + " is not registered");
	}

	inline Response Vendor::run_create_request(const Request& request) const {
		if (request.path().empty()) {
			Response::Body response_data;
			response_data.add("what", server::String("path must contain at least one token"));
			return Response(Response::ResponseCode::NOT_FOUND, response_data);
		}
		auto path(request.path());
		auto resource_id(*(path.begin()));
		if (!contains_resource(resource_id)) {
			Response::Body response_data;
			response_data.add("what", server::String("resource " + resource_id + " is not registered"));
			return Response(Response::ResponseCode::NOT_FOUND, response_data);
		}
		auto resource_request_path(Request::Path(path.begin() + 1, path.end()));
		return access_resource(resource_id)->run_request(Request(request.method(), resource_request_path, request.body()));
	}

	inline Response Vendor::run_read_request(const Request& request) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	inline Response Vendor::run_update_request(const Request& request) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	inline Response Vendor::run_delete_request(const Request& request) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // VENDOR_HPP