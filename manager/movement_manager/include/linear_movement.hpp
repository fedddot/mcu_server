#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"

namespace manager {
	class LinearMovement {
	public:
		LinearMovement(
			const Vector<double>& destination,
			const Vector<double>& basis
		);
		LinearMovement(const LinearMovement& other) = default;
		LinearMovement& operator=(const LinearMovement&) = default;

		std::vector<AxisStep> evaluate_steps() const;
	private:
		Vector<double> m_destination;
		Vector<double> m_basis;
		double m_movement_length;

		bool is_enough(const Vector<double>& current) const;
		AxisStep evaluate_axis_step(const Vector<double>& current) const;
		Vector<double> add_step(const Vector<double>& current, const AxisStep& step) const;
		double evaluate_error_metric(const Vector<double>& current) const;
		
		static double scalar_product(const Vector<double>& one, const Vector<double>& other);
	};

	inline LinearMovement::LinearMovement(
		const Vector<double>& destination,
		const Vector<double>& basis
	): m_destination(destination), m_basis(basis) {
		m_movement_length = std::sqrt(scalar_product(destination, destination));
		if (m_movement_length == 0.0) {
			throw std::runtime_error("destination vector cannot be zero");
		}
	}
	
	inline std::vector<AxisStep> LinearMovement::evaluate_steps() const {
		auto steps = std::vector<AxisStep>();
		auto current_position = Vector<double>(0.0, 0.0, 0.0);
		while (true) {
			std::cout << "Current position: " << current_position.get(Axis::X) << ", " << current_position.get(Axis::Y) << ", " << current_position.get(Axis::Z) << std::endl;
			if (is_enough(current_position)) {
				break;
			}
			const auto axis_step = evaluate_axis_step(current_position);
			if (!axis_step) {
				break;
			}
			steps.push_back(*axis_step);
			current_position = add_step(current_position, *axis_step);
		}
		return steps;
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
		auto evaluate_increment = [this](const AxisStep& step) {
			auto increment = m_basis.get(step.axis);
			if (step.direction == Direction::NEGATIVE) {
				increment = -increment;
			}
			return increment;
		};
		auto result = current;
		result.set(step.axis, result.get(step.axis) + evaluate_increment(step));
		return result;
	}

	inline double LinearMovement::evaluate_error_metric(const Vector<double>& current) const {
		const auto current_len = std::sqrt(scalar_product(current, current));
		if (current_len == 0.0) {
			throw std::runtime_error("current vector cannot be zero for error metric evaluation");
		}
		const auto prod = scalar_product(m_destination, current);
		const auto cos_angle = prod / m_movement_length / current_len;
		const auto sin_angle = std::sqrt(1 - cos_angle * cos_angle);
		return sin_angle;
	}

	inline double LinearMovement::scalar_product(const Vector<double>& one, const Vector<double>& other) {
		auto result = 0.0;
		for (const auto& axis : {Axis::X, Axis::Y, Axis::Z}) {
			result += one.get(axis) * other.get(axis);
		}
		return result;
	}
}

#endif // LINEAR_MOVEMENT_HPP