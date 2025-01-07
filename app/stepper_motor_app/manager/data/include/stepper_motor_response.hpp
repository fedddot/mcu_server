#ifndef	STEPPER_MOTOR_RESPONSE_HPP
#define	STEPPER_MOTOR_RESPONSE_HPP

#include <memory>
#include <stdexcept>

#include "stepper_motor_types.hpp"

namespace manager {
	class StepperMotorResponse {
	public:
		enum class ResultCode: int {
			OK,
			UNSUPPORTED,
			BAD_REQUEST,
			EXCEPTION
		};
		StepperMotorResponse(const ResultCode& code = ResultCode::EXCEPTION);
		StepperMotorResponse(const StepperMotorResponse&);
		StepperMotorResponse& operator=(const StepperMotorResponse&);

		virtual ~StepperMotorResponse() noexcept = default;

		ResultCode code() const;

		bool has_state() const;
		StepperMotorState state() const;
		void set_state(const StepperMotorState& state);
	private:
		ResultCode m_code;
		std::unique_ptr<StepperMotorState> m_state;
	};

	inline StepperMotorResponse::StepperMotorResponse(const ResultCode& code): m_code(code) {

	}

	inline StepperMotorResponse::StepperMotorResponse(const StepperMotorResponse& other): m_code(other.m_code) {
		if (other.m_state) {
			m_state = std::make_unique<StepperMotorState>(*(other.m_state));
		}
	}

	inline StepperMotorResponse& StepperMotorResponse::operator=(const StepperMotorResponse& other) {
		m_code = other.m_code;
		if (other.m_state) {
			m_state = std::make_unique<StepperMotorState>(*(other.m_state));
		}
		return std::ref(*this);
	}

	inline typename StepperMotorResponse::ResultCode StepperMotorResponse::code() const {
		return m_code;
	}

	inline bool StepperMotorResponse::has_state() const {
		return nullptr != m_state;
	}

	inline StepperMotorState StepperMotorResponse::state() const {
		if (!m_state) {
			throw std::runtime_error("state has not been set");
		}
		return *m_state;
	}

	inline void StepperMotorResponse::set_state(const StepperMotorState& state) {
		m_state = std::make_unique<StepperMotorState>(state);
	}
}

#endif // STEPPER_MOTOR_RESPONSE_HPP