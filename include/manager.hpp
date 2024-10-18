#ifndef	MANAGER_HPP
#define	MANAGER_HPP

#include "object.hpp"

namespace server {
	template <typename Tresource_id>
	class Manager {
	public:
		virtual ~Manager() noexcept = default;
		virtual void create_resource(const Tresource_id& id, const server::Object& create_config) = 0;
		virtual server::Object read_resource(const Tresource_id& id) const = 0;
		virtual server::Object read_all_resources() const = 0;
		virtual void update_resource(const Tresource_id& id, const server::Object& update_config) = 0;
		virtual void delete_resource(const Tresource_id& id) = 0;
		virtual bool contains(const Tresource_id& id) const = 0;
	};
}

#endif // MANAGER_HPP