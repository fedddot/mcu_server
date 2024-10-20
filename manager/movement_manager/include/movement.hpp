#ifndef	MOVEMENT_HPP
#define	MOVEMENT_HPP

#include "data.hpp"

namespace manager {
	class Movement {
	public:
		virtual ~Movement() noexcept = default;
		virtual void perform(const server::Data& cfg) = 0;
	};
}

#endif // MOVEMENT_HPP