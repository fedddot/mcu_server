#ifndef	CIRCULAR_MOVEMENT_MODEL_HPP
#define	CIRCULAR_MOVEMENT_MODEL_HPP

#include <stdexcept>

#include "movement_model.hpp"
#include "vector.hpp"

namespace manager {
	template <typename T, typename Ttime, typename Tspeed>
	class CircularMovementModel: MovementModel<T, Ttime> {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		CircularMovementModel(
			const Vector<T>& target,
			const Vector<T>& rotation_center,
			const Direction& direction,
			const Tspeed& speed
		);
		CircularMovementModel(const CircularMovementModel&) = delete;
		CircularMovementModel& operator=(const CircularMovementModel&) = delete;
		Vector<T> evaluate(const Ttime& time) const override;
	private:
		Vector<T> m_target;
		Vector<T> m_rotation_center;
		Direction m_direction;
		Tspeed m_speed;
	};
	
	template <typename T, typename Ttime, typename Tspeed>
	inline CircularMovementModel<T, Ttime, Tspeed>::CircularMovementModel(
		const Vector<T>& target,
		const Vector<T>& rotation_center,
		const Direction& direction,
		const Tspeed& speed
	): m_target(target), m_rotation_center(rotation_center), m_direction(direction), m_speed(speed) {

	}

	template <typename T, typename Ttime, typename Tspeed>
	inline Vector<T> CircularMovementModel<T, Ttime, Tspeed>::evaluate(const Ttime& time) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

}

#endif // CIRCULAR_MOVEMENT_MODEL_HPP