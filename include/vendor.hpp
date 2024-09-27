#ifndef	VENDOR_HPP
#define	VENDOR_HPP

#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include <memory>
#include <string>
#include <vector>

namespace server {
	class Vendor: public Resource {
	public:
		Vendor(const std::string& id);
		Vendor(const Vendor&);
		Vendor& operator=(const Vendor&) = delete;
		
		std::string id() const override;
		Response run_request(const Request&) override;
		void register_resource(const Resource& resource);
		void contains_resource(const std::string& id) const;
	private:
		std::string m_id;
		std::vector<std::unique_ptr<Resource>> m_resources;
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

	inline Response Vendor::run_request(const Request& request) {
		return m_id;
	}
}

#endif // VENDOR_HPP