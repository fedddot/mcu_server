#ifndef	VENDOR_HPP
#define	VENDOR_HPP

#include <memory>
#include <stdexcept>
#include <string>
#include <map>

#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace vendor {
	class Vendor: public server::Resource {
	public:
		Vendor() = default;
		Vendor(const Vendor& other);
		Vendor& operator=(const Vendor&) = delete;
		
		server::Response run_request(const server::Request& request) const override;
		Resource *clone() const override;
		
		void register_resource(const std::string& id, const Resource& resource);
		bool contains_resource(const std::string& id) const;
	private:
		std::map<std::string, std::unique_ptr<Resource>> m_resources;
		Resource *access_resource(const std::string& id) const;
	};

	inline Vendor::Vendor(const Vendor& other) {
		for (const auto& [id, resource_ptr]: other.m_resources) {
			m_resources.insert({id, std::unique_ptr<Resource>(resource_ptr->clone())});
		}
	}

	inline server::Response Vendor::run_request(const server::Request& request) const {
		using namespace server;
		if (request.path().empty()) {
			using namespace server;
			Response::Body response_data;
			response_data.add("what", String("path must contain at least one token"));
			return Response(Response::ResponseCode::BAD_REQUEST, response_data);
		}
		auto path(request.path());
		auto resource_id(*(path.begin()));
		if (!contains_resource(resource_id)) {
			Response::Body response_data;
			response_data.add("what", String("resource " + resource_id + " is not registered"));
			return Response(Response::ResponseCode::NOT_FOUND, response_data);
		}
		auto resource_request_path(Request::Path(path.begin() + 1, path.end()));
		return access_resource(resource_id)->run_request(Request(request.method(), resource_request_path, request.body()));
	}

	inline server::Resource *Vendor::clone() const {
		return new Vendor(*this);
	}

	inline void Vendor::register_resource(const std::string& id, const Resource& resource) {
		if (contains_resource(id)) {
			throw std::invalid_argument("resource id " + id + " is already registered");
		}
		m_resources.insert({id, std::unique_ptr<Resource>(resource.clone())});
	}

	inline bool Vendor::contains_resource(const std::string& id) const {
		return m_resources.end() != m_resources.find(id);
	}

	inline server::Resource *Vendor::access_resource(const std::string& id) const {
		const auto iter = m_resources.find(id);
		if (m_resources.end() == iter) {
			throw std::invalid_argument("resource " + id + " is not registered");
		}
		return (iter->second).get();
	}
}

#endif // VENDOR_HPP