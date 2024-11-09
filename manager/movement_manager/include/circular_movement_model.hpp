#ifndef	CIRCULAR_MOVEMENT_MODEL_HPP
#define	CIRCULAR_MOVEMENT_MODEL_HPP

#include <cmath>
#include <cstdlib>
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
		Vector<double> evaluate() override;
		bool finished() const override;
		double dt() const override;
	private:
		const Vector<double> m_rotation_center;
		const Vector<double> m_rco;
		const Vector<double> m_rct;
		
		double m_dt;
		double m_dphi;
		double m_phi;

		static double dot_product(const Vector<double>& one, const Vector<double>& other);
		static double norm(const Vector<double>& vector);
		static Vector<double> scale(const Vector<double>& vector, const double factor);
		static double angle_between(const Vector<double>& one, const Vector<double>& other);
		static Vector<double> product(const Vector<double>& one, const Vector<double>& other);
		Vector<double> evaluate_current_position() const;
	};

	inline CircularMovementModel::CircularMovementModel(
		const Vector<double>& target,
		const Vector<double>& rotation_center,
		const Direction& direction,
		const double speed,
		const unsigned int steps_per_length
	): m_rotation_center(rotation_center), m_rco(Vector<double>(0, 0, 0) - rotation_center), m_rct(target - rotation_center) {
		using Axis = typename Vector<double>::Axis;
		if (!steps_per_length || !speed) {
			throw std::invalid_argument("invalid args received");
		}
		const auto dl = static_cast<double>(1) / steps_per_length;
		m_dt = std::abs(dl / speed);
		auto speed_sign((direction == Direction::CCW) ? 1 : -1);
		const auto basis_product = product(m_rco, m_rct);
		if (0 > basis_product.projection(Axis::Z)) {
			speed_sign = -speed_sign;
		}

		const auto rotation_radius = norm(rotation_center);
		if (0 == rotation_radius) {
			throw std::invalid_argument("invalid rotation radius");
		}
		m_dphi = static_cast<double>(speed_sign) * dl / rotation_radius;
		m_phi = 0;
	}

	inline Vector<double> CircularMovementModel::evaluate() {
		const auto position = evaluate_current_position() + m_rotation_center;
		m_phi += m_dphi;
		return position;
	}
	
	inline Vector<double> CircularMovementModel::evaluate_current_position() const {
		const auto CO_projection = scale(m_rco, std::cos(m_phi));
		const auto psi = angle_between(m_rco, m_rct) - m_phi;
		const auto CT_projection = scale(m_rct, std::cos(psi));
		return CO_projection + CT_projection;
	}
	
	inline double CircularMovementModel::dt() const {
		return m_dt;
	}
	
	inline bool CircularMovementModel::finished() const {
		const auto eps = m_dphi;
		const auto deviation = angle_between(evaluate_current_position(), m_rct);
		return std::abs(deviation) < std::abs(eps);
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
		double norml2(0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			norml2 += vector.projection(axis) * vector.projection(axis);
		}
		return std::sqrt(norml2);
	}
	
	inline double CircularMovementModel::angle_between(const Vector<double>& one, const Vector<double>& other) {
		const auto product = dot_product(one, other);
		const auto one_norm = norm(one);
		const auto other_norm = norm(other);
		if (!one_norm || !other_norm) {
			return 0;
		}
		const auto cos_alpha = product / one_norm / other_norm;
		return std::acos(cos_alpha);
	}
	
	inline Vector<double> CircularMovementModel::product(const Vector<double>& one, const Vector<double>& other) {
		using Axis = typename Vector<double>::Axis;
		return Vector<double>(
			one.projection(Axis::Y) * other.projection(Axis::Z) - one.projection(Axis::Z) * other.projection(Axis::Y),
			- one.projection(Axis::X) * other.projection(Axis::Z) + one.projection(Axis::Z) * other.projection(Axis::X),
			one.projection(Axis::X) * other.projection(Axis::Y) - one.projection(Axis::Y) * other.projection(Axis::X)
		);
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