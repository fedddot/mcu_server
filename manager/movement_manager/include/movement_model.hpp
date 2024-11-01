#ifndef	MOVEMENT_MODEL_HPP
#define	MOVEMENT_MODEL_HPP

#include "vector.hpp"
#include <tuple>

namespace manager {
	template <typename T, typename Ttime>
	class MovementModel {
	public:
		virtual ~MovementModel() noexcept = default;
		virtual Vector<T> evaluate(const Ttime& time) const = 0;
		virtual Ttime tmax() const = 0;
	};
}

#endif // MOVEMENT_MODEL_HPP