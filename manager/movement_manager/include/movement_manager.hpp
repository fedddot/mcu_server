#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <cmath>
#include <memory>
#include <optional>
#include <stdexcept>

#include "manager_clonable.hpp"
#include "linear_movement.hpp"
#include "manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_manager_vector.hpp"

namespace manager {
	class MovementManager: public ClonableManager<MovementManagerRequest, MovementManagerResponse> {
	public:
		class AxesController {
		public:
			virtual ~AxesController() noexcept = default;
			virtual void step(const AxisStep& step) = 0;
			virtual void enable() = 0;
			virtual void disable() = 0;
			virtual AxesController *clone() const = 0;
		};
		MovementManager(
			const AxesController& axes_controller,
			const AxesProperties& axes_properties
		);
		MovementManager(const MovementManager& other) = default;
		MovementManager& operator=(const MovementManager&) = delete;

		MovementManagerResponse run(const MovementManagerRequest& request) override;
		Manager<MovementManagerRequest, MovementManagerResponse> *clone() const override;
	private:
		std::shared_ptr<AxesController> m_axes_controller;
		AxesProperties m_axes_properties;
		MovementManagerResponse linear_movement(const Vector<double>& destination, const double speed) const;
		MovementManagerResponse circular_movement(const Vector<double>& rotation_center, const double angle, const double speed) const;
	};

	inline MovementManager::MovementManager(
		const AxesController& axes_controller,
		const AxesProperties& axes_properties
	): m_axes_controller(axes_controller.clone()), m_axes_properties(axes_properties) {

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

		auto movement = LinearMovement(
			destination,
			m_axes_properties,
			speed
		);
		m_axes_controller->enable();
		while (true) {
			const auto step = movement.next_step();
			if (!step) {
				break;
			}
			m_axes_controller->step(*step);
		}
		m_axes_controller->disable();
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