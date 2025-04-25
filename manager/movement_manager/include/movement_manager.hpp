#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <cmath>
#include <functional>
#include <optional>
#include <stdexcept>

#include "clonable_manager.hpp"
#include "linear_movement.hpp"
#include "manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_manager_vector.hpp"

namespace manager {
	class MovementManager: public ClonableManager<MovementManagerRequest, MovementManagerResponse> {
	public:
		using AxesController = std::function<void(const AxisStep& step, const double step_duration)>;
		MovementManager(
			const AxesController& axes_controller,
			const AxesProperties& axes_properties
		);
		MovementManager(const MovementManager& other) = default;
		MovementManager& operator=(const MovementManager&) = delete;

		MovementManagerResponse run(const MovementManagerRequest& request) override;
		Manager<MovementManagerRequest, MovementManagerResponse> *clone() const override;
	private:
		AxesController m_axes_controller;
		AxesProperties m_axes_properties;
		MovementManagerResponse linear_movement(const Vector<double>& destination, const double speed) const;
		MovementManagerResponse circular_movement(const Vector<double>& rotation_center, const double angle, const double speed) const;
	};

	inline MovementManager::MovementManager(
		const AxesController& axes_controller,
		const AxesProperties& axes_properties
	): m_axes_controller(axes_controller), m_axes_properties(axes_properties) {
		if (!m_axes_controller) {
			throw std::invalid_argument("invalid axes controller received");
		}
	}
	
	inline MovementManagerResponse MovementManager::run(const MovementManagerRequest& request) {
		switch (request.get_movement_type()) {
		case MovementManagerRequest::MovementType::LINEAR:
			return linear_movement(
				request.get_movement_data<MovementManagerRequest::LinearMovementData>().destination, 
				request.get_movement_data<MovementManagerRequest::LinearMovementData>().speed
			);
		case MovementManagerRequest::MovementType::ROTATIONAL:
			return circular_movement(
				request.get_movement_data<MovementManagerRequest::RotationalMovementData>().rotation_center,
				request.get_movement_data<MovementManagerRequest::RotationalMovementData>().angle,
				request.get_movement_data<MovementManagerRequest::RotationalMovementData>().speed
			);
		default:
			return MovementManagerResponse {
				.code = MovementManagerResponse::ResultCode::BAD_REQUEST,
				.message = "unsupported movement type received",
			};
		}
	}

	inline Manager<MovementManagerRequest, MovementManagerResponse> *MovementManager::clone() const {
		return new MovementManager(*this);
	}

	inline MovementManagerResponse MovementManager::linear_movement(const Vector<double>& destination, const double speed) const {
		if (speed <= 0) {
			throw std::invalid_argument("speed must be a positive non-zero number");
		}
		const auto path_length = std::sqrt(inner_product(destination, destination));
		if (!path_length) {
			return MovementManagerResponse {
				.code = MovementManagerResponse::ResultCode::OK,
				.message = std::nullopt,
			};
		}
		const auto movement = LinearMovement(
			destination,
			m_axes_properties
		);
		const auto steps = movement.evaluate_steps();
		const auto steps_number = steps.size();
		if (steps_number == 0) {
			return MovementManagerResponse {
				.code = MovementManagerResponse::ResultCode::OK,
				.message = std::nullopt,
			};
		}
		const auto total_movement_time = path_length / speed;
		const auto step_duration = total_movement_time / steps_number;
		for (const auto& step: steps) {
			m_axes_controller(step, step_duration);
		}
		return MovementManagerResponse {
			.code = MovementManagerResponse::ResultCode::OK,
			.message = std::nullopt,
		};
	}
	
	inline MovementManagerResponse MovementManager::circular_movement(const Vector<double>& rotation_center, const double angle, const double speed) const {
		throw std::runtime_error("not implemented");
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP