#ifndef	SERVER_HPP
#define	SERVER_HPP

#include "request.hpp"
#include "response.hpp"

namespace server {
	class Server {
	public:
		Server(const Vendor)
		virtual ~Server() noexcept = default;
		virtual Response run_request(const Request& request) const = 0;
		virtual Server *clone() const = 0;
	};
}

#endif // SERVER_HPP