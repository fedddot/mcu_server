#ifndef	STEPPER_MOTOR_DATA_HPP
#define	STEPPER_MOTOR_DATA_HPP

namespace manager {
	enum class Direction: int {
		CW,
		CCW
	};
	enum class State: int {
		DISABLED,
		ENABLED
	};
}

#endif // STEPPER_MOTOR_DATA_HPP