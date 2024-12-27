#ifndef	MANAGER_FACTORY_HPP
#define	MANAGER_FACTORY_HPP

#include "data.hpp"
#include "manager.hpp"

namespace server {
	template <typename Trequest, typename Tresponse>
	class ManagerFactory {
	public:
		virtual ~ManagerFactory() noexcept = default;
		virtual Manager<Trequest, Tresponse> *create(const Data& config) const = 0;
	};
}

#endif // MANAGER_FACTORY_HPP