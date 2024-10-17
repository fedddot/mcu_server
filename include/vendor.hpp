#ifndef	VENDOR_HPP
#define	VENDOR_HPP

#include "request.hpp"
#include "response.hpp"

namespace server {
	class Vendor {
	public:
		virtual ~Vendor() noexcept = default;
		virtual server::Response run_request(const server::Request& request) const = 0;
	};
}

#endif // VENDOR_HPP