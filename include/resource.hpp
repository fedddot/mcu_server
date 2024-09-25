#ifndef	RESOURCE_HPP
#define	RESOURCE_HPP

#include "request.hpp"
#include "response.hpp"

namespace server {
	template <typename Tid>
	class Resource {
	public:
		virtual ~Resource() noexcept = default;
		virtual Tid id() const = 0;
		virtual Response run_request(const Request&) = 0;
	};
}

#endif // RESOURCE_HPP