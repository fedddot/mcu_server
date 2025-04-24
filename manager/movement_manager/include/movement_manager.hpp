#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <stdexcept>

#include "clonable_manager.hpp"
#include "manager.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "vector.hpp"

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
		throw std::runtime_error("not implemented");
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
		throw std::runtime_error("not implemented");
	}

	inline Manager<MovementManagerRequest, MovementManagerResponse> *MovementManager::clone() const {
		return new MovementManager(*this);
	}

	inline MovementManagerResponse MovementManager::linear_movement(const Vector& destination, const double speed) const {
		throw std::runtime_error("not implemented");
	}
	
	inline MovementManagerResponse MovementManager::circular_movement(const Vector& rotation_center, const double angle, const double speed) const {
		throw std::runtime_error("not implemented");
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP