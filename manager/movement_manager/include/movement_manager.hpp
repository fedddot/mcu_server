#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <optional>
#include <stdexcept>

#include "clonable_manager.hpp"
#include "linear_movement.hpp"
#include "manager.hpp"
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

		MovementManagerResponse linear_movement(const Vector& destination, const double speed) const;
		MovementManagerResponse circular_movement(const Vector& rotation_center, const double angle, const double speed) const;
	};

	inline MovementManager::MovementManager(
	) {
	}
	
	inline MovementManagerResponse MovementManager::run(const MovementManagerRequest& request) {
		switch (request.type) {
		case MovementManagerRequest::MovementType::LINEAR:
			return linear_movement(
				request.movement.linear.destination, 
				request.movement.linear.speed
			);
		case MovementManagerRequest::MovementType::ROTATIONAL:
			return circular_movement(
				request.movement.rotational.rotation_center,
				request.movement.rotational.angle,
				request.movement.rotational.speed
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

	inline MovementManagerResponse MovementManager::linear_movement(const Vector& destination, const double speed) const {
		const auto movement = LinearMovement(
			destination,
			speed,
			[](const double distance, const double speed) {
				throw std::runtime_error("not implemented");
			},
			[](const double distance, const double speed) {
				throw std::runtime_error("not implemented");
			},
			[](const double distance, const double speed) {
				throw std::runtime_error("not implemented");
			}
		);
		try {
			movement.perform();
		} catch (const std::exception& e) {
			return MovementManagerResponse {
				.code = MovementManagerResponse::ResultCode::EXCEPTION,
				.message = e.what(),
			};
		}
		return MovementManagerResponse {
			.code = MovementManagerResponse::ResultCode::OK,
			.message = std::nullopt,
		};
	}
	
	inline MovementManagerResponse MovementManager::circular_movement(const Vector& rotation_center, const double angle, const double speed) const {
		throw std::runtime_error("not implemented");
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP