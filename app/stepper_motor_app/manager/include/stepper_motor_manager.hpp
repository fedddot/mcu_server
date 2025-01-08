#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <exception>
#include <map>
#include <memory>
#include <stdexcept>

#include "manager.hpp"
#include "provider.hpp"
#include "providers.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_creator.hpp"
#include "stepper_motor_delay_generator.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"

namespace manager {
	template <typename Tcreate_cfg>
	class StepperMotorManager: public Manager<StepperMotorRequest<Tcreate_cfg>, StepperMotorResponse> {
	public:
		StepperMotorManager(Providers<StepperMotorProviderType> *providers);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest<Tcreate_cfg>& request) override;
	private:
		Providers<StepperMotorProviderType> *m_providers;
		std::map<StepperMotorId, std::unique_ptr<StepperMotor>> m_motors;

		StepperMotorResponse serve_create(const StepperMotorRequest<Tcreate_cfg>& request);
		StepperMotorResponse serve_delete(const StepperMotorRequest<Tcreate_cfg>& request);
		StepperMotorResponse serve_steps(const StepperMotorRequest<Tcreate_cfg>& request);

		template <typename Tin, typename Tout>
		static const Tout& cast_dynamically(const Tin& input);
	};

	template <typename Tcreate_cfg>
	inline StepperMotorManager<Tcreate_cfg>::StepperMotorManager(Providers<StepperMotorProviderType> *providers): m_providers(providers) {
		if (!m_providers) {
			throw std::invalid_argument("invalid providers ptr received");
		}
	}

	template <typename Tcreate_cfg>
	inline StepperMotorResponse StepperMotorManager<Tcreate_cfg>::run(const StepperMotorRequest<Tcreate_cfg>& request) {
		switch (request.type()) {
		case StepperMotorRequest<Tcreate_cfg>::Type::CREATE_STEPPER:
			return serve_create(request);
		case StepperMotorRequest<Tcreate_cfg>::Type::DELETE_STEPPER:
			return serve_delete(request);
		case StepperMotorRequest<Tcreate_cfg>::Type::STEPS:
			return serve_steps(request);
		default:
			return StepperMotorResponse(StepperMotorResponse::ResultCode::UNSUPPORTED);
		}
	}

	template <typename Tcreate_cfg>
	StepperMotorResponse StepperMotorManager<Tcreate_cfg>::serve_create(const StepperMotorRequest<Tcreate_cfg>& request) {
		try {
			const auto& create_provider_raw = m_providers->access_provider(StepperMotorProviderType::MOTOR_CREATOR);
			const auto& create_provider = cast_dynamically<Provider, StepperMotorCreator<Tcreate_cfg>>(create_provider_raw);
			if (!request.has_create_config()) {
				return StepperMotorResponse(StepperMotorResponse::ResultCode::BAD_REQUEST);
			}
			m_motors[request.motor_id()] = std::unique_ptr<StepperMotor>(create_provider.create(request.create_config()));
			return StepperMotorResponse(StepperMotorResponse::ResultCode::OK);
		} catch (const std::exception& e) {
			auto response = StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
			response.set_message("an exception caught in StepperMotorManager::serve_create: " + std::string(e.what()));
			return response;
		}
	}

	template <typename Tcreate_cfg>
	StepperMotorResponse StepperMotorManager<Tcreate_cfg>::serve_delete(const StepperMotorRequest<Tcreate_cfg>& request) {
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

	template <typename Tcreate_cfg>
	StepperMotorResponse StepperMotorManager<Tcreate_cfg>::serve_steps(const StepperMotorRequest<Tcreate_cfg>& request) {
		try {
			if (!request.has_steps()) {
				return StepperMotorResponse(StepperMotorResponse::ResultCode::BAD_REQUEST); 
			}
			const auto steps = request.steps();
			const auto iter = m_motors.find(request.motor_id());
			if (m_motors.end() == iter) {
				return StepperMotorResponse(StepperMotorResponse::ResultCode::NOT_FOUND);
			}
			const auto& delay_provider_raw = m_providers->access_provider(StepperMotorProviderType::DELAY_GENERATOR);
			const auto& delay_provider = cast_dynamically<Provider, StepperMotorDelayGenerator>(delay_provider_raw);

			auto remaining_steps = steps.steps_number;
			iter->second->enable();
			while (remaining_steps) {
				iter->second->step(steps.direction);
				delay_provider.delay(steps.step_duration);
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

	template <typename Tcreate_cfg>
	template <typename Tin, typename Tout>
	const Tout& StepperMotorManager<Tcreate_cfg>::cast_dynamically(const Tin& input) {
		try {
			return dynamic_cast<const Tout&>(input);
		} catch (...) {
			throw std::invalid_argument("failed to perform dynamic downcast");
		}
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP