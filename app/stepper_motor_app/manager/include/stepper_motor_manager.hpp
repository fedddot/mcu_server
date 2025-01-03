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

		void create_stepper(const StepperMotorId& id, const Tcreate_cfg& create_cfg);
	};

	template <typename Tcreate_cfg>
	inline StepperMotorManager<Tcreate_cfg>::StepperMotorManager(Providers<StepperMotorProviderType> *providers, const StepperMotorManagerConfig& manager_cfg): m_providers(providers), m_manager_cfg(manager_cfg) {
		if (!m_providers) {
			throw std::invalid_argument("invalid providers ptr received");
		}
	}

	template <typename Tcreate_cfg>
	inline StepperMotorResponse StepperMotorManager<Tcreate_cfg>::run(const StepperMotorRequest& request) {
		return StepperMotorResponse(StepperMotorResponseCode::EXCEPTION);
	}

	template <typename Tcreate_cfg>
	inline const Providers<StepperMotorProviderType>& StepperMotorManager<Tcreate_cfg>::providers() const {
		return std::ref(*m_providers);
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP