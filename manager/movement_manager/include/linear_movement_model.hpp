#ifndef	LINEAR_MOVEMENT_MODEL_HPP
#define	LINEAR_MOVEMENT_MODEL_HPP

#include <cmath>
#include <stdexcept>

#include "movement_model.hpp"
#include "vector.hpp"

namespace manager {
	class LinearMovementModel: MovementModel<double, double> {
	public:
		LinearMovementModel(
			const Vector<double>& target,
			const double speed,
			const unsigned int steps_per_length
		);
		LinearMovementModel(const LinearMovementModel&) = delete;
		LinearMovementModel& operator=(const LinearMovementModel&) = delete;
		Vector<double> evaluate() override;
		double dt() const override;
		bool finished() const override;
	private:
		const Vector<double> m_target;
		const Vector<double> m_velocity;
		const double m_time_max;
		const double m_step_duration;
		double m_time;
		
		Vector<double> evaluate_current_position() const;
		static Vector<double> calculate_velocity(const Vector<double>& target, const double speed);
		static double calculate_time_max(const Vector<double>& target, const double speed);
		static double calculate_step_duration(const double speed, const unsigned int steps_per_length);
		static double norm(const Vector<double>& vector);
	};

	inline LinearMovementModel::LinearMovementModel(
		const Vector<double>& target,
		const double speed,
		const unsigned int steps_per_length
	): m_target(target), m_velocity(calculate_velocity(target, speed)), m_time_max(calculate_time_max(target, speed)), m_step_duration(calculate_step_duration(speed, steps_per_length)), m_time(0) {
	
	}

	inline Vector<double> LinearMovementModel::evaluate() {
		const auto position = evaluate_current_position();
		m_time += m_step_duration;
		return position;
	}
	
	inline bool LinearMovementModel::finished() const {
		return m_time > m_time_max;
	}

	inline double LinearMovementModel::dt() const {
		return m_step_duration;
	}

	inline Vector<double> LinearMovementModel::evaluate_current_position() const {
		using Axis = typename Vector<double>::Axis;
		Vector<double> position(0, 0, 0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			position.set_projection(axis, m_time * m_velocity.projection(axis));
		}
		return position;
	}

	inline Vector<double> LinearMovementModel::calculate_velocity(const Vector<double>& target, const double speed) {
		using Axis = typename Vector<double>::Axis;
		const auto trajectory_length = norm(target);
		if (0 == trajectory_length) {
			throw std::invalid_argument("trajectory length is zero");
		}
		Vector<double> velocity(0, 0, 0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			velocity.set_projection(axis, speed * target.projection(axis) / trajectory_length);
		}
		return velocity;
	}

	inline double LinearMovementModel::calculate_time_max(const Vector<double>& target, const double speed) {
		if (0 == speed) {
			throw std::invalid_argument("invalid speed received");
		}
		return std::abs(norm(target) / speed);
	}
	
	inline double LinearMovementModel::calculate_step_duration(const double speed, const unsigned int steps_per_length) {
		if ((0 == steps_per_length) || (0 == speed)) {
			throw std::invalid_argument("invalid arguments received");
		}
		const auto dl = static_cast<double>(1) / steps_per_length;
		return dl / speed;
	}

	inline double LinearMovementModel::norm(const Vector<double>& vector) {
		using Axis = typename Vector<double>::Axis;
		double norml2(0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			norml2 += vector.projection(axis) * vector.projection(axis);
		}
		return std::sqrt(norml2);
	}
}

#endif // LINEAR_MOVEMENT_MODEL_HPP