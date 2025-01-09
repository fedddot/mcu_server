#ifndef	STEPPER_MOTOR_DELAY_GENERATOR_HPP
#define	STEPPER_MOTOR_DELAY_GENERATOR_HPP

#include <cstddef>

#include "provider.hpp"

namespace manager {
	class StepperMotorDelayGenerator: public Provider {
	public:
		virtual void delay(const std::size_t& timeout_ms) const = 0;
		virtual StepperMotorDelayGenerator *clone() const = 0;
	};
}

#endif // STEPPER_MOTOR_DELAY_GENERATOR_HPP