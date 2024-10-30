#ifndef	CIRCULAR_MOVEMENT_MODEL_HPP
#define	CIRCULAR_MOVEMENT_MODEL_HPP

#include <cmath>
#include <stdexcept>
#include <vector>

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
		float m_rotation_speed;
	};
	
	template <typename T, typename Ttime, typename Tspeed>
	inline CircularMovementModel<T, Ttime, Tspeed>::CircularMovementModel(
		const Vector<T>& target,
		const Vector<T>& rotation_center,
		const Direction& direction,
		const Tspeed& speed
	): m_target(target), m_rotation_center(rotation_center) {
		using Axis = typename Vector<T>::Axis;
		auto norm = [](const Vector<T>& vector) {
			T normL2(0);
			for (const auto& axis: std::vector<Axis>{Axis::X, Axis::Y, Axis::Z}) {
				normL2 += vector.projection(axis) * vector.projection(axis);
			}
			return std::sqrt(normL2);
		};
		const auto rotation_center_distance = norm(m_rotation_center);
		if (0 == rotation_center_distance) {
			throw std::invalid_argument("invalid rotation center received (it's norm is zero)");
		}
		const auto dir_coefficient((direction == Direction::CCW) ? 1 : -1);
		m_rotation_speed = static_cast<float>(dir_coefficient * speed / rotation_center_distance);
	}

	template <typename T, typename Ttime, typename Tspeed>
	inline Vector<T> CircularMovementModel<T, Ttime, Tspeed>::evaluate(const Ttime& time) const {
		using Axis = typename Vector<T>::Axis;
		const auto phi = m_rotation_speed * time;
		const auto cos_phi = std::cos(phi);
		const auto sin_phi = std::sin(phi);
		return m_target.scale(sin_phi) + m_rotation_center.scale(1 - sin_phi - cos_phi);
	}

}

#endif // CIRCULAR_MOVEMENT_MODEL_HPP