#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include "stepper_motor_data.hpp"

namespace manager {
	class StepperMotor {
	public:
		virtual ~StepperMotor() noexcept = default;
		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual bool enabled() const = 0;
		virtual void step(const StepperMotorDirection& direction) = 0;
	};
}

#endif // STEPPER_MOTOR_HPP