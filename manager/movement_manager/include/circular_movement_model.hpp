#ifndef	CIRCULAR_MOVEMENT_MODEL_HPP
#define	CIRCULAR_MOVEMENT_MODEL_HPP

#include <cmath>
#include <cstdlib>
#include <functional>
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
		const double m_radius;

		std::function<Vector<double>(const Vector<double>&)> m_transform_to_base;
		std::function<bool(const double)> m_is_finished;
		double m_dt;
		double m_dphi;
		double m_phi;

		static double dot_product(const Vector<double>& one, const Vector<double>& other);
		static double norm(const Vector<double>& vector);
		static double angle_between(const Vector<double>& one, const Vector<double>& other);
		static Vector<double> evaluate_in_centered_coordinates(const double radius, const double phi);
	};

	inline CircularMovementModel::CircularMovementModel(
		const Vector<double>& target,
		const Vector<double>& rotation_center,
		const Direction& direction,
		const double speed,
		const unsigned int steps_per_length
	): m_radius(norm(rotation_center)) {
		using Axis = typename Vector<double>::Axis;
		if (!steps_per_length || !speed) {
			throw std::invalid_argument("invalid args received");
		}
		if (!m_radius) {
			throw std::invalid_argument("curvature radius can't be zero");
		}
		const auto cos_alpha = rotation_center.projection(Axis::X) / m_radius;
		const auto sin_alpha = rotation_center.projection(Axis::Y) / m_radius;
		const auto dl = static_cast<double>(1) / steps_per_length;
		m_transform_to_base = [cos_alpha, sin_alpha, rotation_center](const Vector<double>& vector) {
			const auto rotated_vector = Vector<double>(
				cos_alpha * vector.projection(Axis::X) - sin_alpha * vector.projection(Axis::Y),
				sin_alpha * vector.projection(Axis::X) + cos_alpha * vector.projection(Axis::Y),
				vector.projection(Axis::Z)
			);
			return rotation_center + rotated_vector;
		};
		auto transform_to_centered = [cos_alpha, sin_alpha](const Vector<double>& vector) {
			return Vector<double>(
				cos_alpha * vector.projection(Axis::X) + sin_alpha * vector.projection(Axis::Y),
				-sin_alpha * vector.projection(Axis::X) + cos_alpha * vector.projection(Axis::Y),
				vector.projection(Axis::Z)
			);

		};
		m_dt = std::abs(dl / speed);
		const auto rct = transform_to_centered(target - rotation_center);
		const auto rco = transform_to_centered(Vector<double>(0, 0, 0) - rotation_center);
		auto phi_max = angle_between(rct, rco);
		if (0 < rct.projection(Axis::Y)) {
			phi_max = static_cast<double>(2) * M_PI - phi_max;
		}
		switch (direction) {
		case Direction::CCW:
			m_phi = 0;
			m_dphi = dl / m_radius;
			m_is_finished = [phi_max](const double phi) {
				return phi > phi_max;
			};
			break;
		case Direction::CW:
			m_phi = static_cast<double>(2) * M_PI;
			m_dphi = - dl / m_radius;
			m_is_finished = [phi_max](const double phi) {
				return phi < phi_max;
			};
			break;
		default:
			throw std::invalid_argument("invalid direction received");
		}
	}

	inline Vector<double> CircularMovementModel::evaluate() {
		const auto centered_position = evaluate_in_centered_coordinates(m_radius, m_phi);
		m_phi += m_dphi;
		const auto transformed_position = m_transform_to_base(centered_position);
		return transformed_position;
	}
	
	inline Vector<double> CircularMovementModel::evaluate_in_centered_coordinates(const double radius, const double phi) {
		return Vector<double>(
			-std::abs(radius) * static_cast<double>(std::cos(phi)),
			-std::abs(radius) * static_cast<double>(std::sin(phi)),
			0
		);
	}

	inline double CircularMovementModel::dt() const {
		return m_dt;
	}
	
	inline bool CircularMovementModel::finished() const {
		return m_is_finished(m_phi);
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
	
}

#endif // CIRCULAR_MOVEMENT_MODEL_HPP