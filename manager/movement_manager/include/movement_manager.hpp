#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <cmath>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>

#include "axes_controller.hpp"
#include "init_request.hpp"
#include "manager_instance.hpp"
#include "linear_movement.hpp"
#include "manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "linear_movement_request.hpp"

namespace manager {
	template <typename AxisControllerConfig>
	class MovementManager: public Manager<MovementManagerRequest, MovementManagerResponse> {
	public:
		using AxesControllerCreator = std::function<Instance<AxesController>(const AxisControllerConfig&)>;
		MovementManager(
			const AxesControllerCreator& axes_controller_ctor,
			const AxesProperties& axes_properties
		);
		MovementManager(const MovementManager& other) = default;
		MovementManager& operator=(const MovementManager&) = delete;

		MovementManagerResponse run(const MovementManagerRequest& request) override;
	private:
		AxesControllerCreator m_axes_controller_ctor;
		std::optional<Instance<AxesController>> m_axes_controller;
		AxesProperties m_axes_properties;
		MovementManagerResponse init(const MovementManagerRequest& request);
		MovementManagerResponse linear_movement(const MovementManagerRequest& request);
		MovementManagerResponse circular_movement(const MovementManagerRequest& request) const;

		template <typename T> 
		static const T& downcast_request(const MovementManagerRequest& request);
	};

	template <typename AxisControllerConfig>
	inline MovementManager<AxisControllerConfig>::MovementManager(
		const AxesControllerCreator& axes_controller_ctor,
		const AxesProperties& axes_properties
	): m_axes_controller_ctor(axes_controller_ctor), m_axes_controller(std::nullopt), m_axes_properties(axes_properties) {
		if (!m_axes_controller_ctor) {
			throw std::invalid_argument("invalid axes_controller_ctor received");
		}
	}
	
	template <typename AxisControllerConfig>
	inline MovementManagerResponse MovementManager<AxisControllerConfig>::run(const MovementManagerRequest& request) {
		switch (request.type()) {
		case MovementManagerRequest::RequestType::INIT:
			return init(request);
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
	inline MovementManagerResponse MovementManager<AxisControllerConfig>::init(const MovementManagerRequest& request) {
		const auto& request_casted = downcast_request<InitRequest<AxisControllerConfig>>(request);
		m_axes_controller = m_axes_controller_ctor(request_casted.axis_controller_config());
		return MovementManagerResponse {
			.code = MovementManagerResponse::ResultCode::OK,
			.message = std::nullopt,
		};
	}

	template <typename AxisControllerConfig>
	inline MovementManagerResponse MovementManager<AxisControllerConfig>::linear_movement(const MovementManagerRequest& request) {
		if (!m_axes_controller) {
			return MovementManagerResponse {
				.code = MovementManagerResponse::ResultCode::EXCEPTION,
				.message = "axes controller is not initialized",
			};
		}
		const auto& request_casted = downcast_request<LinearMovementRequest>(request);
		const auto speed = request_casted.speed();
		const auto destination = request_casted.destination();
		auto movement = LinearMovement(
			destination,
			m_axes_properties,
			speed
		);
		m_axes_controller->get().enable();
		while (true) {
			const auto step = movement.next_step();
			if (!step) {
				break;
			}
			m_axes_controller->get().step(*step);
		}
		m_axes_controller->get().disable();
		return MovementManagerResponse {
			.code = MovementManagerResponse::ResultCode::OK,
			.message = std::nullopt,
		};
	}
	
	template <typename AxisControllerConfig>
	inline MovementManagerResponse MovementManager<AxisControllerConfig>::circular_movement(const MovementManagerRequest& request) const {
		throw std::runtime_error("not implemented");
	}

	template <typename AxisControllerConfig>
	template <typename T>
	inline const T& MovementManager<AxisControllerConfig>::downcast_request(const MovementManagerRequest& request) {
		try {
			return dynamic_cast<const T&>(request);
		} catch (...) {
			throw std::invalid_argument("failed to cast received request into target type");
		}
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP