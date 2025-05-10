#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <cmath>
#include <memory>
#include <optional>
#include <stdexcept>

#include "axes_controller.hpp"
#include "manager_clonable.hpp"
#include "linear_movement.hpp"
#include "manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "linear_movement_request.hpp"

namespace manager {
	template <typename AxisControllerConfig>
	class MovementManager: public Manager<MovementManagerRequest, MovementManagerResponse>, public Clonable<Manager<MovementManagerRequest, MovementManagerResponse>> {
	public:
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
		MovementManagerResponse linear_movement(const MovementManagerRequest& request) const;
		MovementManagerResponse circular_movement(const MovementManagerRequest& request) const;
	};

	template <typename AxisControllerConfig>
	inline MovementManager<AxisControllerConfig>::MovementManager(
		const AxesController& axes_controller,
		const AxesProperties& axes_properties
	): m_axes_controller(axes_controller.clone()), m_axes_properties(axes_properties) {

	}
	
	template <typename AxisControllerConfig>
	inline MovementManagerResponse MovementManager<AxisControllerConfig>::run(const MovementManagerRequest& request) {
		switch (request.type()) {
		case MovementManagerRequest::RequestType::LINEAR_MOVEMENT:
			return linear_movement(request);
		case MovementManagerRequest::RequestType::ROTATIONAL_MOVEMENT:
			return circular_movement(request);
		default:
			return MovementManagerResponse {
				.code = MovementManagerResponse::ResultCode::BAD_REQUEST,
				.message = "unsupported movement type received",
			};
		}
	}

	template <typename AxisControllerConfig>
	inline Manager<MovementManagerRequest, MovementManagerResponse> *MovementManager<AxisControllerConfig>::clone() const {
		return new MovementManager(*this);
	}

	template <typename AxisControllerConfig>
	inline MovementManagerResponse MovementManager<AxisControllerConfig>::linear_movement(const MovementManagerRequest& request) const {
		const auto& request_casted = dynamic_cast<const LinearMovementRequest&>(request);
		const auto speed = request_casted.speed();
		const auto destination = request_casted.destination();
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
	
	template <typename AxisControllerConfig>
	inline MovementManagerResponse MovementManager<AxisControllerConfig>::circular_movement(const MovementManagerRequest& request) const {
		throw std::runtime_error("not implemented");
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP