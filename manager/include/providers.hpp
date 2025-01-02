#ifndef	PROVIDERS_HPP
#define	PROVIDERS_HPP

#include "provider.hpp"

namespace manager {
	template <typename Tprovider_id>
	class Providers {
	public:
		virtual ~Providers() noexcept = default;
		virtual Provider& access_provider(const Tprovider_id& id) const = 0;
	};
}

#endif // PROVIDERS_HPP