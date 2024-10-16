#ifndef	MANAGER_HPP
#define	MANAGER_HPP

#include "server_types.hpp"

namespace server {
	class Manager {
	public:
		virtual ~Manager() noexcept = default;
		virtual void create_resource(const Body& create_request_body) = 0;
		virtual Body read_resource(const Path& route) const = 0;
		virtual Body read_all_resources() const = 0;
		virtual void update_resource(const Path& route, const Body& update_request_body) = 0;
		virtual void delete_resource(const Path& route) = 0;
		virtual bool contains(const Path& route) const = 0;
	};
}

#endif // MANAGER_HPP