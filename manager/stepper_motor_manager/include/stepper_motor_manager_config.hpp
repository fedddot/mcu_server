#ifndef	STEPPER_MOTOR_MANAGER_CONFIG_HPP
#define	STEPPER_MOTOR_MANAGER_CONFIG_HPP

#include <memory>

#include "manager_config.hpp"
#include "stepper_motor_creator.hpp"
#include "stepper_motor_delay_generator.hpp"

namespace manager {
	template <typename StepperCreateConfig>
	class StepperMotorManagerConfig: public ManagerConfig {
	public:
		StepperMotorManagerConfig(
			const StepperMotorCreator<StepperCreateConfig>& motor_ctor,
			const StepperMotorDelayGenerator& delay_generator
		);
		StepperMotorManagerConfig(const StepperMotorManagerConfig& other);
		StepperMotorManagerConfig& operator=(const StepperMotorManagerConfig& other);

		std::string type() const override;
		const StepperMotorCreator<StepperCreateConfig>& motor_ctor() const;
		const StepperMotorDelayGenerator& delay_generator() const;
	private:
		std::unique_ptr<StepperMotorCreator<StepperCreateConfig>> m_motor_ctor;
		std::unique_ptr<StepperMotorDelayGenerator> m_delay_generator;
	};

	template <typename StepperCreateConfig>
	inline StepperMotorManagerConfig<StepperCreateConfig>::StepperMotorManagerConfig(
		const StepperMotorCreator<StepperCreateConfig>& motor_ctor,
		const StepperMotorDelayGenerator& delay_generator
	): m_motor_ctor(motor_ctor.clone()), m_delay_generator(delay_generator.clone()) {

	}

	template <typename StepperCreateConfig>
	inline StepperMotorManagerConfig<StepperCreateConfig>::StepperMotorManagerConfig(const StepperMotorManagerConfig& other): m_motor_ctor(other.m_motor_ctor->clone()), m_delay_generator(other.m_delay_generator->clone()) {
		
	}

	template <typename StepperCreateConfig>
	inline StepperMotorManagerConfig<StepperCreateConfig>& StepperMotorManagerConfig<StepperCreateConfig>::operator=(const StepperMotorManagerConfig& other) {
		m_motor_ctor = std::unique_ptr<StepperMotorCreator<StepperCreateConfig>>(other.m_motor_ctor->clone());
		m_delay_generator = std::unique_ptr<StepperMotorDelayGenerator>(other.m_delay_generator->clone());
	}

	template <typename StepperCreateConfig>
	inline std::string StepperMotorManagerConfig<StepperCreateConfig>::type() const {
		return std::string("manager.stepper_motor");
	}

	template <typename StepperCreateConfig>
	inline const StepperMotorCreator<StepperCreateConfig>& StepperMotorManagerConfig<StepperCreateConfig>::motor_ctor() const {
		return std::ref(*m_motor_ctor);
	}
		
	template <typename StepperCreateConfig>
	inline const StepperMotorDelayGenerator& StepperMotorManagerConfig<StepperCreateConfig>::delay_generator() const {
		return std::ref(*m_delay_generator);
	}
}

#endif // STEPPER_MOTOR_MANAGER_CONFIG_HPP