#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"

namespace manager {
	class LinearMovement {
	public:
		LinearMovement(
			const Vector<double>& destination,
			const AxesProperties& axes_properties
		);
		LinearMovement(const LinearMovement& other) = default;
		LinearMovement& operator=(const LinearMovement&) = default;

		std::vector<AxisStep> evaluate_steps() const;
	private:
		Vector<double> m_destination;
		AxesProperties m_axes_properties;
		double m_movement_length;

		bool is_enough(const Vector<double>& current) const;
		AxisStep evaluate_axis_step(const Vector<double>& current) const;
		Vector<double> add_step(const Vector<double>& current, const AxisStep& step) const;
		static double evaluate_error_metric(const Vector<double>& base_vector, const Vector<double>& vector);
		
		static double inner_product(const Vector<double>& one, const Vector<double>& other);
		static Vector<double> vector_product(const Vector<double>& one, const Vector<double>& other);
	};

	inline LinearMovement::LinearMovement(
		const Vector<double>& destination,
		const AxesProperties& axes_properties
	): m_destination(destination), m_axes_properties(axes_properties) {
		m_movement_length = std::sqrt(inner_product(destination, destination));
		if (m_movement_length == 0.0) {
			throw std::runtime_error("destination vector cannot be zero");
		}
	}
	
	inline std::vector<AxisStep> LinearMovement::evaluate_steps() const {
		auto steps = std::vector<AxisStep>();
		auto current_position = Vector<double>(0.0, 0.0, 0.0);
		while (true) {
			if (is_enough(current_position)) {
				break;
			}
			const auto axis_step = evaluate_axis_step(current_position);
			steps.push_back(axis_step);
			current_position = add_step(current_position, axis_step);
		}
		return steps;
	}

	inline bool LinearMovement::is_enough(const Vector<double>& current) const {
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			const auto difference = m_destination.get(axis) - current.get(axis);
			if ((m_destination.get(axis) >= 0) && (difference < 0)) {
				return true;
			}
			if ((m_destination.get(axis) < 0) && (difference >= 0)) {
				return true;
			}
			if (std::abs(difference) > std::abs(m_destination.get(axis))) {
				return true;
			}
		}
		return false;
	}

	inline AxisStep LinearMovement::evaluate_axis_step(const Vector<double>& current) const {
		auto best_step = AxisStep {};
		auto best_error_metric = std::numeric_limits<double>::max();
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			const auto direction = (m_destination.get(axis) >= 0) ? Direction::POSITIVE : Direction::NEGATIVE;
			const auto step_target = add_step(current, AxisStep {axis, direction});
			const auto error_metric = evaluate_error_metric(m_destination, step_target);
			if (error_metric < best_error_metric) {
				best_error_metric = error_metric;
				best_step = AxisStep {axis, direction};
			}
		}
		return best_step;
	}

	inline Vector<double> LinearMovement::add_step(const Vector<double>& current, const AxisStep& step) const {
		auto increment = m_axes_properties.get_step_length(step.axis);
		if (step.direction == Direction::NEGATIVE) {
			increment = -increment;
		}
		auto result = current;
		result.set(step.axis, result.get(step.axis) + increment);
		return result;
	}

	inline double LinearMovement::evaluate_error_metric(const Vector<double>& base_vector, const Vector<double>& vector) {
		const auto vector_prod = vector_product(base_vector, vector);
		const auto prod_norm_L2 = inner_product(vector_prod, vector_prod);
		return prod_norm_L2;
	}

	inline double LinearMovement::inner_product(const Vector<double>& one, const Vector<double>& other) {
		auto result = 0.0;
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			result += one.get(axis) * other.get(axis);
		}
		return result;
	}

	inline Vector<double> LinearMovement::vector_product(const Vector<double>& one, const Vector<double>& other) {
		return Vector<double>(
			one.get(Axis::Y) * other.get(Axis::Z) - one.get(Axis::Z) * other.get(Axis::Y),
			- one.get(Axis::X) * other.get(Axis::Z) + one.get(Axis::Z) * other.get(Axis::X),
			one.get(Axis::X) * other.get(Axis::Y) - one.get(Axis::Y) * other.get(Axis::X)
		);
	}
}

#endif // LINEAR_MOVEMENT_HPP