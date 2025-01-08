#ifndef	STEPPER_MOTOR_TYPES_HPP
#define	STEPPER_MOTOR_TYPES_HPP

#include <string>

namespace manager {
	using StepperMotorId = std::string;

	enum class StepperMotorDirection: int {
		CW,
		CCW
	};

	enum class StepperMotorState: int {
		DISABLED,
		ENABLED
	};

	enum class StepperMotorProviderType: int {
		MOTOR_CREATOR,
		DELAY_GENERATOR
	};

	using StepperMotorManagerConfig = std::string;
}

#endif // STEPPER_MOTOR_TYPES_HPP