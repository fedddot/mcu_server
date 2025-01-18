#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <exception>
#include <map>
#include <memory>
#include <stdexcept>

#include "manager.hpp"
#include "stepper_motor_manager_config.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"

namespace manager {
	template <typename StepperCreateConfig>
	class StepperMotorManager: public Manager<StepperMotorRequest<StepperCreateConfig>, StepperMotorResponse> {
	public:
		StepperMotorManager(const StepperMotorManagerConfig<StepperCreateConfig>& config);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest<StepperCreateConfig>& request) override;
	private:
		StepperMotorManagerConfig<StepperCreateConfig> m_config;
		std::map<StepperMotorId, std::unique_ptr<StepperMotor>> m_motors;

		StepperMotorResponse serve_create(const StepperMotorRequest<StepperCreateConfig>& request);
		StepperMotorResponse serve_delete(const StepperMotorRequest<StepperCreateConfig>& request);
		StepperMotorResponse serve_steps(const StepperMotorRequest<StepperCreateConfig>& request);

		template <typename Tin, typename Tout>
		static const Tout& cast_dynamically(const Tin& input);
	};

	template <typename StepperCreateConfig>
	inline StepperMotorManager<StepperCreateConfig>::StepperMotorManager(const StepperMotorManagerConfig<StepperCreateConfig>& config): m_config(config) {
		
	}

	template <typename StepperCreateConfig>
	inline StepperMotorResponse StepperMotorManager<StepperCreateConfig>::run(const StepperMotorRequest<StepperCreateConfig>& request) {
		switch (request.type()) {
		case StepperMotorRequest<StepperCreateConfig>::Type::CREATE_STEPPER:
			return serve_create(request);
		case StepperMotorRequest<StepperCreateConfig>::Type::DELETE_STEPPER:
			return serve_delete(request);
		case StepperMotorRequest<StepperCreateConfig>::Type::STEPS:
			return serve_steps(request);
		default:
			return StepperMotorResponse(StepperMotorResponse::ResultCode::UNSUPPORTED);
		}
	}

	template <typename StepperCreateConfig>
	StepperMotorResponse StepperMotorManager<StepperCreateConfig>::serve_create(const StepperMotorRequest<StepperCreateConfig>& request) {
		try {
			if (!request.has_create_config()) {
				return StepperMotorResponse(StepperMotorResponse::ResultCode::BAD_REQUEST);
			}
			m_motors[request.motor_id()] = std::unique_ptr<StepperMotor>(m_config.motor_ctor().create(request.create_config()));
			return StepperMotorResponse(StepperMotorResponse::ResultCode::OK);
		} catch (const std::exception& e) {
			auto response = StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
			response.set_message("an exception caught in StepperMotorManager::serve_create: " + std::string(e.what()));
			return response;
		}
	}

	template <typename StepperCreateConfig>
	StepperMotorResponse StepperMotorManager<StepperCreateConfig>::serve_delete(const StepperMotorRequest<StepperCreateConfig>& request) {
		try {
			const auto iter = m_motors.find(request.motor_id());
			if (m_motors.end() == iter) {
				return StepperMotorResponse(StepperMotorResponse::ResultCode::NOT_FOUND);
			}
			m_motors.erase(iter);
			return StepperMotorResponse(StepperMotorResponse::ResultCode::OK);
		} catch (const std::exception& e) {
			auto response = StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
			response.set_message("an exception caught in StepperMotorManager::serve_delete: " + std::string(e.what()));
			return response;
		}
	}

	template <typename StepperCreateConfig>
	StepperMotorResponse StepperMotorManager<StepperCreateConfig>::serve_steps(const StepperMotorRequest<StepperCreateConfig>& request) {
		try {
			if (!request.has_steps()) {
				return StepperMotorResponse(StepperMotorResponse::ResultCode::BAD_REQUEST); 
			}
			const auto steps = request.steps();
			const auto iter = m_motors.find(request.motor_id());
			if (m_motors.end() == iter) {
				return StepperMotorResponse(StepperMotorResponse::ResultCode::NOT_FOUND);
			}
			auto remaining_steps = steps.steps_number;
			iter->second->enable();
			while (remaining_steps) {
				iter->second->step(steps.direction);
				m_config.delay_generator().delay(steps.step_duration);
				--remaining_steps;
			}
			iter->second->disable();
			return StepperMotorResponse(StepperMotorResponse::ResultCode::OK);
		} catch (const std::exception& e) {
			auto response = StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
			response.set_message("an exception caught in StepperMotorManager::serve_steps: " + std::string(e.what()));
			return response;
		}
	}

	template <typename StepperCreateConfig>
	template <typename Tin, typename Tout>
	const Tout& StepperMotorManager<StepperCreateConfig>::cast_dynamically(const Tin& input) {
		try {
			return dynamic_cast<const Tout&>(input);
		} catch (...) {
			throw std::invalid_argument("failed to perform dynamic downcast");
		}
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP