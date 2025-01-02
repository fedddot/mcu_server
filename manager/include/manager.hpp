#ifndef	MANAGER_HPP
#define	MANAGER_HPP

#include "providers.hpp"

namespace manager {
	template <typename Trequest, typename Tresponse, typename Tprovider_id>
	class Manager {
	public:
		virtual ~Manager() noexcept = default;
		virtual Tresponse run(const Trequest& request) = 0;
		virtual const Providers<Tprovider_id>& providers() const = 0;
	};
}

#endif // MANAGER_HPP