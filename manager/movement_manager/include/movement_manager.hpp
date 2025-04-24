#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <stdexcept>

#include "clonable_manager.hpp"
#include "manager.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"

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
	};

	inline MovementManager::MovementManager(
	) {
		throw std::runtime_error("not implemented");
	}
	
	inline MovementManagerResponse MovementManager::run(const MovementManagerRequest& request) {
		throw std::runtime_error("not implemented");
	}

	inline Manager<MovementManagerRequest, MovementManagerResponse> *MovementManager::clone() const {
		return new MovementManager(*this);
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP