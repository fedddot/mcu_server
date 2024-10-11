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
		virtual void steps(const Direction& direction, const unsigned int steps_num, const unsigned int on_time, const unsigned int off_time) = 0;
		virtual StepperMotor *clone() const = 0;
	};
}

#endif // STEPPER_MOTOR_HPP