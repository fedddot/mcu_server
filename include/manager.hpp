#ifndef	MANAGER_HPP
#define	MANAGER_HPP

#include <stdexcept>
#include <string>

#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"

namespace server {
	class Manager: public Resource {
	public:
		Manager(const std::string& id);
		Manager(const Manager&) = default;
		Manager& operator=(const Manager&) = default;
		
		std::string id() const override;
		Response run_request(const Request&) override;
		Resource *clone() const override;
	private:
		std::string m_id;
	};

	inline Manager::Manager(const std::string& id): m_id(id) {

	}

	inline std::string Manager::id() const {
		return m_id;
	}

	inline Response Manager::run_request(const Request& request) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	inline Resource *Manager::clone() const {
		return new Manager(*this);
	}
}

#endif // MANAGER_HPP