#ifndef	STEPPER_MOTOR_RESPONSE_HPP
#define	STEPPER_MOTOR_RESPONSE_HPP

#include <optional>
#include <string>

#include "stepper_motor_data.hpp"

namespace manager {
	struct StepperMotorResponse {
		enum class ResultCode: int {
			OK,
			NOT_FOUND,
			UNSUPPORTED,
			BAD_REQUEST,
			EXCEPTION
		};
		StepperMotorResponse() = default;
		StepperMotorResponse(const StepperMotorResponse&) = default;
		StepperMotorResponse& operator=(const StepperMotorResponse&) = default;

		ResultCode code;
		std::optional<State> state;
		std::optional<std::string> message;
	};
	
	inline bool operator==(const StepperMotorResponse& lhs, const StepperMotorResponse& rhs) {
		return lhs.code == rhs.code &&
			lhs.state == rhs.state &&
			lhs.message == rhs.message;
	}
}

#endif // STEPPER_MOTOR_RESPONSE_HPP