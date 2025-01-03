#ifndef	STEPPER_MOTOR_CREATOR_HPP
#define	STEPPER_MOTOR_CREATOR_HPP

#include "provider.hpp"
#include "stepper_motor.hpp"

namespace manager {
	template <typename Tcreate_cfg>
	class StepperMotorCreator: public Provider {
	public:
		virtual StepperMotor *create(const Tcreate_cfg& create_cfg) const = 0;
	};
}

#endif // STEPPER_MOTOR_CREATOR_HPP