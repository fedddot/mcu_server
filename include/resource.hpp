#ifndef	RESOURCE_HPP
#define	RESOURCE_HPP

#include <string>

#include "request.hpp"
#include "response.hpp"

namespace server {
	class Resource {
	public:
		virtual ~Resource() noexcept = default;
		virtual std::string id() const = 0;
		virtual Response run_request(const Request&) = 0;
	};
}

#endif // RESOURCE_HPP