#ifndef	MOVEMENT_HPP
#define	MOVEMENT_HPP

#include "data.hpp"

namespace manager {
	class Movement {
	public:
		enum class Type: int {
			LINEAR,
			CIRCULAR
		};
		virtual ~Movement() noexcept = default;
		virtual void perform(const manager::Data& cfg) = 0;
		virtual Type type() const = 0;
	};
}

#endif // MOVEMENT_HPP