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
		const Vector<double> m_target;
		const Vector<double> m_rotation_center;
		const unsigned int m_steps_per_length;
		const double m_radius;
		
		double m_rotation_speed;
		double m_step_duration;
		double m_phi;

		static double dot_product(const Vector<double>& one, const Vector<double>& other);
		static double norm(const Vector<double>& vector);
		static Vector<double> scale(const Vector<double>& vector, const double factor);
		static double angle_between(const Vector<double>& one, const Vector<double>& other);
		Vector<double> evaluate_current_position() const;
	};

	inline CircularMovementModel::CircularMovementModel(
		const Vector<double>& target,
		const Vector<double>& rotation_center,
		const Direction& direction,
		const double speed,
		const unsigned int steps_per_length
	): m_target(target), m_rotation_center(rotation_center), m_steps_per_length(steps_per_length), m_radius(norm(rotation_center)) {
		using Axis = typename Vector<double>::Axis;
		if (!m_steps_per_length || !m_radius || !speed) {
			throw std::invalid_argument("invalid args received");
		}
		const auto speed_sign((direction == Direction::CCW) ? 1 : -1);
		m_rotation_speed = static_cast<double>(speed_sign) * std::abs(speed) / m_radius;
		if (0 == m_rotation_speed) {
			throw std::invalid_argument("rotation speed mustn't be zero");
		}
		const auto dl = static_cast<double>(1) / m_steps_per_length;
		m_step_duration = dl / speed;
		if (0 == m_step_duration) {
			throw std::invalid_argument("step duration mustn't be zero");
		}
		m_phi = 0;
	}

	inline Vector<double> CircularMovementModel::evaluate() {
		const auto position = evaluate_current_position();
		m_phi += m_rotation_speed * m_step_duration;
		return position;
	}	
	
	inline Vector<double> CircularMovementModel::evaluate_current_position() const {
		const auto CO = Vector<double>(0, 0, 0) - m_rotation_center;
		const auto CT = m_target - m_rotation_center;
		const auto OCT = angle_between(CO, CT);
		const auto CO_projection = scale(CO, std::cos(m_phi));
		const auto CT_projection = scale(CT, std::cos(OCT - m_phi));
		return CO_projection + CT_projection + m_rotation_center;
	}
	
	inline double CircularMovementModel::dt() const {
		return m_step_duration;
	}
	
	inline bool CircularMovementModel::finished() const {
		const auto CO = Vector<double>(0, 0, 0) - m_rotation_center;
		const auto CT = m_target - m_rotation_center;
		const auto OCT = angle_between(CO, CT);

		const auto cycles = static_cast<int>(m_phi / (2 * M_PI));
		auto phi_scaled = m_phi - cycles * 2 * M_PI;
		if (0 > phi_scaled) {
			phi_scaled = 2 * M_PI + phi_scaled;
		}
		const auto epsilon = std::abs(m_step_duration * m_rotation_speed);
		return std::abs(phi_scaled - OCT) < epsilon;
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