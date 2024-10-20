#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

namespace manager {
	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		virtual ~StepperMotor() noexcept = default;
		virtual void enable() = 0;
		virtual void disable() = 0;
		virtual bool enabled() const = 0;
		virtual void step(const Direction& direction) = 0;
	};
}

#endif // STEPPER_MOTOR_HPP