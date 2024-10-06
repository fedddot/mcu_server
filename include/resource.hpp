#ifndef	RESOURCE_HPP
#define	RESOURCE_HPP

#include "request.hpp"
#include "response.hpp"

namespace server {
	class Resource {
	public:
		virtual ~Resource() noexcept = default;
		virtual Response run_request(const Request& request) const = 0;
		virtual Resource *clone() const = 0;
	};
}

#endif // RESOURCE_HPP