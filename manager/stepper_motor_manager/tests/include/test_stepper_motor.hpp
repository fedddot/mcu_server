#ifndef	TEST_STEPPER_MOTOR_HPP
#define	TEST_STEPPER_MOTOR_HPP

#include <functional>
#include <stdexcept>

#include "stepper_motor.hpp"

namespace manager_uts {
	class TestStepperMotor: public manager::StepperMotor {
	public:
		using StepsAction = std::function<void(const Direction&, unsigned int, unsigned int)>;
		TestStepperMotor(const StepsAction& action): m_action(action) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}
		TestStepperMotor(const TestStepperMotor&) = default;
		TestStepperMotor& operator=(const TestStepperMotor&) = delete;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) override {
			m_action(direction, steps_num, step_duration_ms);
		}
		manager::StepperMotor *clone() const override {
			return new TestStepperMotor(*this);
		}
	private:
		StepsAction m_action;
	};
}

#endif // TEST_STEPPER_MOTOR_HPP