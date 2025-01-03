#ifndef	STEPPER_MOTOR_RESPONSE_HPP
#define	STEPPER_MOTOR_RESPONSE_HPP

#include <memory>
#include <stdexcept>

#include "stepper_motor_types.hpp"

namespace manager {

	enum class StepperMotorResponseCode: int {
		OK,
		UNSUPPORTED,
		EXCEPTION
	};

	class StepperMotorResponse {
	public:
		StepperMotorResponse(const StepperMotorResponseCode& code = StepperMotorResponseCode::EXCEPTION);
		StepperMotorResponse(const StepperMotorResponse&);
		StepperMotorResponse& operator=(const StepperMotorResponse&);

		virtual ~StepperMotorResponse() noexcept = default;

		const StepperMotorResponseCode code() const;
		void set_code(const StepperMotorResponseCode& code);
		const StepperMotorState state() const;
		const bool has_state() const;
		void set_state(const StepperMotorState& state);
	private:
		StepperMotorResponseCode m_code;
		std::unique_ptr<StepperMotorState> m_state;
	};

	inline StepperMotorResponse::StepperMotorResponse(const StepperMotorResponseCode& code): m_code(code), m_state(nullptr) {

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

	inline const StepperMotorResponseCode StepperMotorResponse::code() const {
		return m_code;
	}

	inline void StepperMotorResponse::set_code(const StepperMotorResponseCode& code) {
		m_code = code;
	}
	inline const StepperMotorState StepperMotorResponse::state() const {
		if (!m_state) {
			throw std::runtime_error("state has not been set");
		}
		return *m_state;
	}
	inline const bool StepperMotorResponse::has_state() const {
		return nullptr != m_state;
	}
	inline void StepperMotorResponse::set_state(const StepperMotorState& state) {
		m_state = std::make_unique<StepperMotorState>(state);
	}
}

#endif // STEPPER_MOTOR_RESPONSE_HPP