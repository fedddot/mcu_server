#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

namespace manager {
	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		enum class State: int {
			DISABLED,
			ENABLED
		};
		virtual ~StepperMotor() noexcept = default;
		virtual void set_state(const State& state) = 0;
		virtual State state() const = 0;
		virtual void step(const Direction& direction) = 0;
	};
}

#endif // STEPPER_MOTOR_HPP