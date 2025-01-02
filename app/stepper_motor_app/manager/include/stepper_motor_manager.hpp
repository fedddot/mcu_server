#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "manager.hpp"
#include "providers.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_data.hpp"
#include "string.hpp"

namespace manager {
	class StepperMotorManager: public Manager<StepperMotorRequest, StepperMotorResponse, StepperMotorProviderType> {
	public:
		StepperMotorManager(Providers<StepperMotorProviderType> *providers, const Data& manager_cfg);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest& request) override;
		const Providers<StepperMotorProviderType>& providers() const override;
	private:
		Providers<StepperMotorProviderType> *m_providers;
		std::unique_ptr<Data> m_manager_cfg;
		std::map<StepperMotorId, std::unique_ptr<StepperMotor>> m_motors;
	};

	inline StepperMotorManager::StepperMotorManager(Providers<StepperMotorProviderType> *providers, const Data& manager_cfg): m_providers(providers), m_manager_cfg(manager_cfg.clone()) {
		if (!m_providers) {
			throw std::invalid_argument("invalid providers ptr received");
		}
	}

	inline StepperMotorResponse StepperMotorManager::run(const StepperMotorRequest& request) {
		return StepperMotorResponse(StepperMotorResponseCode::EXCEPTION, String("NOT IMPLEMENTED"));
	}

	inline const Providers<StepperMotorProviderType>& StepperMotorManager::providers() const {
		return std::ref(*m_providers);
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP