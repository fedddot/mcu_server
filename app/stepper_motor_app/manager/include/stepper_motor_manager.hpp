#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "manager.hpp"
#include "provider.hpp"
#include "providers.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_creator.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_request_data.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_types.hpp"

namespace manager {
	template <typename Tcreate_cfg>
	class StepperMotorManager: public Manager<StepperMotorRequest, StepperMotorResponse> {
	public:
		StepperMotorManager(Providers<StepperMotorProviderType> *providers);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest& request) override;
	private:
		Providers<StepperMotorProviderType> *m_providers;
		std::map<StepperMotorId, std::unique_ptr<StepperMotor>> m_motors;

		StepperMotorResponse serve_create(const StepperMotorRequestData& data);

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
	inline StepperMotorResponse StepperMotorManager<Tcreate_cfg>::run(const StepperMotorRequest& request) {
		if (!request.has_data()) {
			return StepperMotorResponse(StepperMotorResponse::ResultCode::BAD_REQUEST);
		}
		switch (request.type()) {
		case StepperMotorRequest::Type::CREATE_STEPPER:
			return serve_create(request.data());
		default:
			return StepperMotorResponse(StepperMotorResponse::ResultCode::UNSUPPORTED);
		}
	}

	template <typename Tcreate_cfg>
	StepperMotorResponse StepperMotorManager<Tcreate_cfg>::serve_create(const StepperMotorRequestData& data) {
		try {
			const auto& create_provider_raw = m_providers->access_provider(StepperMotorProviderType::MOTOR_CREATOR);
			const auto& create_provider = cast_dynamically<Provider, StepperMotorCreator<Tcreate_cfg>>(create_provider_raw);
			const auto& create_config = cast_dynamically<StepperMotorRequestData, StepperMotorCreateRequestData<StepperMotorId, Tcreate_cfg>>(data);

			m_motors[create_config.motor_id()] = std::unique_ptr<StepperMotor>(create_provider.create(create_config.create_cfg()));
			return StepperMotorResponse(StepperMotorResponse::ResultCode::OK);
		} catch (...) {
			return StepperMotorResponse(StepperMotorResponse::ResultCode::EXCEPTION);
		}
	}

	template <typename Tcreate_cfg>
	template <typename Tin, typename Tout>
	const Tout& StepperMotorManager<Tcreate_cfg>::cast_dynamically(const Tin& input) {
		try {
			return dynamic_cast<const Tout&>(input);
		} catch (...) {
			throw std::invalid_argument("failed to perform dynamic downcast in stepper motor manager");
		}
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP