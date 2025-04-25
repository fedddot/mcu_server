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
		double evaluate_error_metric(const Vector<double>& current) const;
		
		static double inner_product(const Vector<double>& one, const Vector<double>& other);
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
			steps.push_back(*axis_step);
			current_position = add_step(current_position, axis_step);
		}
		return steps;
	}

	inline bool LinearMovement::is_enough(const Vector<double>& current) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
	
	inline AxisStep LinearMovement::evaluate_axis_step(const Vector<double>& current) const {
		auto best_step = AxisStep {};
		auto best_error_metric = std::numeric_limits<double>::max();
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			const auto direction = (m_destination.get(axis) >= 0) ? Direction::POSITIVE : Direction::NEGATIVE;
			const auto step_target = add_step(current, AxisStep {axis, direction});
			const auto error_metric = evaluate_error_metric(step_target);
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

	inline double LinearMovement::evaluate_error_metric(const Vector<double>& current) const {
		const auto current_len = std::sqrt(inner_product(current, current));
		if (current_len == 0.0) {
			throw std::runtime_error("current vector cannot be zero for error metric evaluation");
		}
		const auto prod_curr_dest = inner_product(m_destination, current);
		const auto cos_curr_dest = prod_curr_dest / m_movement_length / current_len;
		const auto sin_curr_dest = std::sqrt(1 - cos_curr_dest * cos_curr_dest);
		return sin_curr_dest;
	}

	inline double LinearMovement::inner_product(const Vector<double>& one, const Vector<double>& other) {
		auto result = 0.0;
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			result += one.get(axis) * other.get(axis);
		}
		return result;
	}
}

#endif // LINEAR_MOVEMENT_HPP