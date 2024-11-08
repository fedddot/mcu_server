#ifndef	MOVEMENT_MODEL_HPP
#define	MOVEMENT_MODEL_HPP

#include "vector.hpp"

namespace manager {
	template <typename T, typename Ttime>
	class MovementModel {
	public:
		virtual ~MovementModel() noexcept = default;
		virtual Vector<T> evaluate() = 0;
		virtual bool finished() const = 0;
		virtual Ttime dt() const = 0;
	};
}

#endif // MOVEMENT_MODEL_HPP