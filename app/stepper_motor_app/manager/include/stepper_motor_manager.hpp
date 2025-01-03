#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "manager.hpp"
#include "providers.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_types.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"
#include "stepper_motor_creator.hpp"

namespace manager {
	template <typename Tcreate_cfg>
	class StepperMotorManager: public Manager<StepperMotorRequest, StepperMotorResponse, StepperMotorProviderType> {
	public:
		StepperMotorManager(Providers<StepperMotorProviderType> *providers, const StepperMotorManagerConfig& manager_cfg);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest& request) override;
		const Providers<StepperMotorProviderType>& providers() const override;
	private:
		Providers<StepperMotorProviderType> *m_providers;
		StepperMotorManagerConfig m_manager_cfg;
		std::map<StepperMotorId, std::unique_ptr<StepperMotor>> m_motors;

		StepperMotorResponse create_stepper(const CreateStepperMotorRequest<Tcreate_cfg>& create_req);
	};

	template <typename Tcreate_cfg>
	inline StepperMotorManager<Tcreate_cfg>::StepperMotorManager(Providers<StepperMotorProviderType> *providers, const StepperMotorManagerConfig& manager_cfg): m_providers(providers), m_manager_cfg(manager_cfg) {
		if (!m_providers) {
			throw std::invalid_argument("invalid providers ptr received");
		}
	}

	template <typename Tcreate_cfg>
	inline StepperMotorResponse StepperMotorManager<Tcreate_cfg>::run(const StepperMotorRequest& request) {
		switch (request.type()) {
		case StepperMotorRequestType::CREATE_STEPPER:
			return create_stepper(StepperMotorRequest::cast<CreateStepperMotorRequest<Tcreate_cfg>>(request));
		default:
			return StepperMotorResponse(StepperMotorResponseCode::UNSUPPORTED);
		}
	}

	template <typename Tcreate_cfg>
	inline const Providers<StepperMotorProviderType>& StepperMotorManager<Tcreate_cfg>::providers() const {
		return std::ref(*m_providers);
	}

	template <typename Tcreate_cfg>
	StepperMotorResponse StepperMotorManager<Tcreate_cfg>::create_stepper(const CreateStepperMotorRequest<Tcreate_cfg>& create_req) {
		if (m_motors.end() != m_motors.find(create_req.id())) {
			return StepperMotorResponse(StepperMotorResponseCode::EXCEPTION);
		}
		try {
			auto& create_provider_raw = m_providers->access_provider(StepperMotorProviderType::MOTOR_CREATOR);
			auto& create_provider = dynamic_cast<StepperMotorCreator<Tcreate_cfg>&>(create_provider_raw);
			m_motors[create_req.id()] = std::unique_ptr<StepperMotor>(create_provider.create(create_req.create_data()));
			return StepperMotorResponse(StepperMotorResponseCode::OK);
		} catch (...) {
			return StepperMotorResponse(StepperMotorResponseCode::EXCEPTION);
		}
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP