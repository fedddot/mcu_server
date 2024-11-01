#ifndef	CIRCULAR_MOVEMENT_MODEL_HPP
#define	CIRCULAR_MOVEMENT_MODEL_HPP

#include <cmath>
#include <stdexcept>

#include "movement_model.hpp"
#include "vector.hpp"

namespace manager {
	class CircularMovementModel: MovementModel<int, unsigned int> {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		CircularMovementModel(
			const Vector<int>& target,
			const Vector<int>& rotation_center,
			const Direction& direction,
			const unsigned int& speed
		);
		CircularMovementModel(const CircularMovementModel&) = delete;
		CircularMovementModel& operator=(const CircularMovementModel&) = delete;
		Vector<int> evaluate(const unsigned int& time) const override;
		unsigned int tmax() const override;
	private:
		Vector<int> m_target;
		Vector<int> m_rotation_center;
		double m_rotation_speed;
		double m_phi_max;
		double m_radius;

		static long dot_product(const Vector<int>& one, const Vector<int>& other);
		static double norm(const Vector<int>& vector);
		static Vector<int> scale(const Vector<int>& vector, const double factor);
		double opening_angle() const; 
	};


	inline CircularMovementModel::CircularMovementModel(
		const Vector<int>& target,
		const Vector<int>& rotation_center,
		const Direction& direction,
		const unsigned int& speed
	): m_target(target), m_rotation_center(rotation_center) {
		using Axis = typename Vector<int>::Axis;
		const auto radius(norm(m_rotation_center));
		if (0 == radius) {
			throw std::invalid_argument("rotation radius can't be zero");
		}
		m_radius = radius;
		const auto speed_sign((direction == Direction::CCW) ? -1 : 1);
		m_rotation_speed = static_cast<double>(speed_sign * speed) / m_radius;
		m_phi_max = opening_angle();
	}

	inline Vector<int> CircularMovementModel::evaluate(const unsigned int& time) const {
		using Axis = typename Vector<int>::Axis;
		
		const auto phi = m_rotation_speed * time;
		const auto roc = m_rotation_center;
		const auto rtoc_projection = scale(roc, -std::cos(phi)); 
		
		const auto psi = m_phi_max - phi;
		const auto rca = m_target - m_rotation_center;
		const auto rtca_projection = scale(rca, std::cos(psi));

		return rtoc_projection + rtca_projection + roc;
	}

	inline unsigned int CircularMovementModel::tmax() const {
		return static_cast<unsigned int>(m_phi_max / m_rotation_speed);
	}

	inline long CircularMovementModel::dot_product(const Vector<int>& one, const Vector<int>& other) {
		using Axis = typename Vector<int>::Axis;
		long res(0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			res += one.projection(axis) * other.projection(axis);
		}
		return res;
	}

	inline double CircularMovementModel::norm(const Vector<int>& vector) {
		using Axis = typename Vector<int>::Axis;
		long norml2(0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			norml2 += vector.projection(axis) * vector.projection(axis);
		}
		return std::sqrt(norml2);
	}
	
	inline double CircularMovementModel::opening_angle() const {
		const auto pi = 3.14159265358979323846;
		const auto rco = m_rotation_center.negate();
		const auto rca = m_target - m_rotation_center;
		const auto product = dot_product(rco, rca);
		const auto cos_alpha = static_cast<double>(product) / (m_radius * m_radius);
		const auto inner_angle = std::acos(cos_alpha);
		if (0 > m_rotation_speed) {
			return 2 * pi - inner_angle;
		}
		return inner_angle;
	}

	inline Vector<int> CircularMovementModel::scale(const Vector<int>& vector, double factor) {
		using Axis = typename Vector<int>::Axis;
		Vector<int> res(0, 0, 0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			res.set_projection(axis, vector.projection(axis) * factor);
		}
		return res;
	}
}

#endif // CIRCULAR_MOVEMENT_MODEL_HPP