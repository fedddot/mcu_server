#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

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
		MovementManager(
		);
		MovementManager(const MovementManager& other) = default;
		MovementManager& operator=(const MovementManager&) = delete;

		MovementManagerResponse run(const MovementManagerRequest& request) override;
		Manager<MovementManagerRequest, MovementManagerResponse> *clone() const override;
	private:
		AxesProperties m_axes_properties;
		MovementManagerResponse linear_movement(const Vector<double>& destination, const double speed) const;
		MovementManagerResponse circular_movement(const Vector<double>& rotation_center, const double angle, const double speed) const;
	};

	inline MovementManager::MovementManager(
	): m_axes_properties(0.1, 0.1, 0.1) {
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
		const auto movement = LinearMovement(
			destination,
			m_axes_properties
		);
		const auto steps = movement.evaluate_steps();
		throw std::runtime_error("not implemented");
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