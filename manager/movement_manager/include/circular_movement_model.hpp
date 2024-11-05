#ifndef	CIRCULAR_MOVEMENT_MODEL_HPP
#define	CIRCULAR_MOVEMENT_MODEL_HPP

#include <cmath>
#include <stdexcept>

#include "movement_model.hpp"
#include "vector.hpp"

namespace manager {
	class CircularMovementModel: MovementModel<double, double> {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		CircularMovementModel(
			const Vector<double>& target,
			const Vector<double>& rotation_center,
			const Direction& direction,
			const double speed,
			const unsigned int steps_per_length
		);
		CircularMovementModel(const CircularMovementModel&) = delete;
		CircularMovementModel& operator=(const CircularMovementModel&) = delete;
		Vector<double> evaluate(const double& time) const override;
		double tmax() const override;
		double dt() const override;
	private:
		Vector<double> m_target;
		Vector<double> m_rotation_center;
		double m_rotation_speed;
		double m_phi_max;
		double m_radius;
		unsigned int m_steps_per_length;

		static double dot_product(const Vector<double>& one, const Vector<double>& other);
		static double norm(const Vector<double>& vector);
		static Vector<double> scale(const Vector<double>& vector, const double factor);
		double opening_angle() const; 
	};

	inline CircularMovementModel::CircularMovementModel(
		const Vector<double>& target,
		const Vector<double>& rotation_center,
		const Direction& direction,
		const double speed,
		const unsigned int steps_per_length
	): m_target(target), m_rotation_center(rotation_center), m_steps_per_length(steps_per_length) {
		using Axis = typename Vector<double>::Axis;
		const auto radius(norm(m_rotation_center));
		if (0 == radius) {
			throw std::invalid_argument("rotation radius mustn't be zero");
		}
		const auto speed_sign((direction == Direction::CCW) ? -1 : 1);
		const auto rotation_speed = static_cast<double>(speed_sign) * speed / radius;
		if (0 == rotation_speed) {
			throw std::invalid_argument("rotation speed mustn't be zero");
		}
		m_radius = radius;
		m_rotation_speed = rotation_speed;
		m_phi_max = opening_angle();
	}

	inline Vector<double> CircularMovementModel::evaluate(const double& time) const {
		using Axis = typename Vector<double>::Axis;
		
		const auto phi = m_rotation_speed * time;
		const auto roc = m_rotation_center;
		const auto rtoc_projection = scale(roc, -std::cos(phi)); 
		
		const auto psi = m_phi_max - phi;
		const auto rca = m_target - m_rotation_center;
		const auto rtca_projection = scale(rca, std::cos(psi));

		return rtoc_projection + rtca_projection + roc;
	}

	inline double CircularMovementModel::tmax() const {
		return std::abs(m_phi_max / m_rotation_speed);
	}
	
	inline double CircularMovementModel::dt() const {
		const auto dl = static_cast<double>(1) / m_steps_per_length;
		const auto v = m_radius * m_rotation_speed;
		if (0 == v) {
			throw std::runtime_error("velocity mustn't be zero");
		}
		return std::abs(dl / v);
	}

	inline double CircularMovementModel::dot_product(const Vector<double>& one, const Vector<double>& other) {
		using Axis = typename Vector<double>::Axis;
		double res(0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			res += one.projection(axis) * other.projection(axis);
		}
		return res;
	}

	inline double CircularMovementModel::norm(const Vector<double>& vector) {
		using Axis = typename Vector<double>::Axis;
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
		const auto cos_alpha = product / (m_radius * m_radius);
		const auto inner_angle = std::acos(cos_alpha);
		if (0 > m_rotation_speed) {
			return -inner_angle;
		}
		return inner_angle;
	}

	inline Vector<double> CircularMovementModel::scale(const Vector<double>& vector, double factor) {
		using Axis = typename Vector<double>::Axis;
		Vector<double> res(0, 0, 0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			res.set_projection(axis, vector.projection(axis) * factor);
		}
		return res;
	}
}

#endif // CIRCULAR_MOVEMENT_MODEL_HPP