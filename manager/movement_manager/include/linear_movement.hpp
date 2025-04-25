#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>
#include <stdexcept>

#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"

namespace manager {
	class LinearMovement {
	public:
		LinearMovement(
			const Vector<double>& destination,
			const AxesProperties& axes_properties,
			const double speed
		);
		LinearMovement(const LinearMovement& other) = delete;
		LinearMovement& operator=(const LinearMovement&) = delete;

		std::optional<AxisStep> next_step();
	private:
		const Vector<double> m_destination;
		const AxesProperties m_axes_properties;
		const double m_step_duration;

		Vector<double> m_current;

		bool is_enough() const;
		static double evaluate_step_duration(const Vector<double>& destination, const AxesProperties& axes_properties, const double speed);
		static std::size_t evaluate_steps_number(const Vector<double>& destination, const AxesProperties& axes_properties);
		AxisStep evaluate_axis_step(const Vector<double>& current) const;
		Vector<double> add_step(const Vector<double>& current, const AxisStep& step) const;
		static double evaluate_error_metric(const Vector<double>& base_vector, const Vector<double>& vector);
	};

	inline LinearMovement::LinearMovement(
		const Vector<double>& destination,
		const AxesProperties& axes_properties,
		const double speed
	): m_destination(destination), m_axes_properties(axes_properties), m_step_duration(evaluate_step_duration(destination, axes_properties, speed)), m_current(0.0, 0.0, 0.0) {
		
	}
	
	inline std::optional<AxisStep> LinearMovement::next_step() {
		if (is_enough()) {
			return std::nullopt;
		}
		const auto axis_step = evaluate_axis_step(m_current);
		m_current = add_step(m_current, axis_step);
		return axis_step;
	}

	inline bool LinearMovement::is_enough() const {
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			const auto difference = m_destination.get(axis) - m_current.get(axis);
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
				best_step = AxisStep {axis, direction, m_step_duration};
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

	inline double LinearMovement::evaluate_step_duration(const Vector<double>& destination, const AxesProperties& axes_properties, const double speed) {
		if (speed <= 0) {
			throw std::invalid_argument("speed must be a positive non-zero value");
		}
		const auto path_length = std::sqrt(inner_product(destination, destination));
		const auto total_movement_time = path_length / speed;
		const auto steps_number = evaluate_steps_number(destination, axes_properties);
		if (!steps_number) {
			return 0;
		}
		return total_movement_time / steps_number;
	}

	inline std::size_t LinearMovement::evaluate_steps_number(const Vector<double>& destination, const AxesProperties& axes_properties) {
		auto result = std::size_t(0UL);
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			const auto steps_per_axis = static_cast<std::size_t>(destination.get(axis) / axes_properties.get_step_length(axis));
			result += steps_per_axis;
		}
		return result;
	}
}

#endif // LINEAR_MOVEMENT_HPP