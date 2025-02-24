#ifndef	STEPPER_MOTOR_RESPONSE_HPP
#define	STEPPER_MOTOR_RESPONSE_HPP

#include <optional>
#include <string>

#include "stepper_motor.hpp"

namespace manager {
	class StepperMotorResponse {
	public:
		enum class ResultCode: int {
			OK,
			NOT_FOUND,
			UNSUPPORTED,
			BAD_REQUEST,
			EXCEPTION
		};
		StepperMotorResponse(const ResultCode& code = ResultCode::EXCEPTION);
		StepperMotorResponse(const StepperMotorResponse&) = default;
		StepperMotorResponse& operator=(const StepperMotorResponse&) = default;

		virtual ~StepperMotorResponse() noexcept = default;

		ResultCode code() const;

		std::optional<StepperMotor::State> state() const;
		void set_state(const StepperMotor::State& state);

		std::optional<std::string> message() const;
		void set_message(const std::string& message);
	private:
		ResultCode m_code;
		std::optional<StepperMotor::State> m_state;
		std::optional<std::string> m_message;
	};

	inline StepperMotorResponse::StepperMotorResponse(const ResultCode& code): m_code(code) {

	}

	inline typename StepperMotorResponse::ResultCode StepperMotorResponse::code() const {
		return m_code;
	}

	inline std::optional<StepperMotor::State> StepperMotorResponse::state() const {
		return m_state;
	}

	inline void StepperMotorResponse::set_state(const StepperMotor::State& state) {
		m_state = state;
	}

	inline std::optional<std::string> StepperMotorResponse::message() const {
		return m_message;
	}

	inline void StepperMotorResponse::set_message(const std::string& message) {
		m_message = message;
	}
}

#endif // STEPPER_MOTOR_RESPONSE_HPP