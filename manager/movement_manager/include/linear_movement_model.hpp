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
		Vector<double> evaluate(const double& time) const override;
		double tmax() const override;
		double dt() const override;
	private:
		Vector<double> m_target;
		double m_speed;
		unsigned int m_steps_per_length;
		double m_length;
		static double norm(const Vector<double>& vector);
	};

	inline LinearMovementModel::LinearMovementModel(
		const Vector<double>& target,
		const double speed,
		const unsigned int steps_per_length
	): m_target(target), m_speed(speed), m_steps_per_length(steps_per_length), m_length(norm(target)) {
		if ((0 == m_speed) || (0 == m_steps_per_length)) {
			throw std::invalid_argument("invalid args received");
		}
	}

	inline Vector<double> LinearMovementModel::evaluate(const double& time) const {
		using Axis = typename Vector<double>::Axis;
		Vector<double> result(0, 0, 0);
		const auto total_time(tmax());
		if (0 == total_time) {
			throw std::runtime_error("total movement time mustn't be zero");
		}
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			result.set_projection(axis, m_target.projection(axis) * time / total_time);
		}
		return result;
	}

	inline double LinearMovementModel::tmax() const {
		return std::abs(m_length / m_speed);
	}

	inline double LinearMovementModel::dt() const {
		const auto dl = static_cast<double>(1) / m_steps_per_length;
		return std::abs(dl / m_speed);
	}

	inline double LinearMovementModel::norm(const Vector<double>& vector) {
		using Axis = typename Vector<double>::Axis;
		long norml2(0);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			norml2 += vector.projection(axis) * vector.projection(axis);
		}
		return std::sqrt(norml2);
	}
}

#endif // LINEAR_MOVEMENT_MODEL_HPP