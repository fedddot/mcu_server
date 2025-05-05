#ifndef	STEPPER_MOTOR_REQUEST_HPP
#define	STEPPER_MOTOR_REQUEST_HPP

#include <cstddef>
#include <string>

#include "stepper_motor_data.hpp"

namespace manager {
	struct StepperMotorRequest {
		std::string motor_id;
		RotationDirection direction;
		std::size_t steps_number;
		std::size_t step_duration_ms;
	};

	inline bool operator==(const StepperMotorRequest& lhs, const StepperMotorRequest& rhs) {
		return lhs.direction == rhs.direction && 
			lhs.steps_number == rhs.steps_number &&
			lhs.step_duration_ms == rhs.step_duration_ms;
	}
}

#endif // STEPPER_MOTOR_REQUEST_HPP