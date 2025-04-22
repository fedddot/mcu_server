#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include "stepper_motor_data.hpp"

namespace manager {
	class StepperMotor {
	public:
		virtual ~StepperMotor() noexcept = default;
		virtual void set_state(const State& state) = 0;
		virtual State state() const = 0;
		virtual void step(const Direction& direction) = 0;
	};
}

#endif // STEPPER_MOTOR_HPP