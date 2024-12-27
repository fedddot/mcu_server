#ifndef	MANAGER_FACTORY_HPP
#define	MANAGER_FACTORY_HPP

#include "manager.hpp"

namespace manager {
	template <typename Trequest, typename Tresponse, typename Tconfig>
	class ManagerFactory {
	public:
		virtual ~ManagerFactory() noexcept = default;
		virtual Manager<Trequest, Tresponse> *create(const Tconfig& config) const = 0;
	};
}

#endif // MANAGER_FACTORY_HPP