#ifndef	TEST_STEPPER_MOTOR_HPP
#define	TEST_STEPPER_MOTOR_HPP

#include <functional>
#include <stdexcept>

#include "stepper_motor.hpp"

namespace manager_uts {
	class TestStepperMotor: public manager::StepperMotor {
	public:
		using StepsAction = std::function<void(const Direction&, unsigned int, unsigned int, unsigned int)>;
		TestStepperMotor(const StepsAction& action): m_action(action) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}
		TestStepperMotor(const TestStepperMotor&) = default;
		TestStepperMotor& operator=(const TestStepperMotor&) = delete;

		void steps(const Direction& direction, const unsigned int steps_num, const unsigned int on_time, const unsigned int off_time) override {
			m_action(direction, steps_num, on_time, off_time);
		}
		manager::StepperMotor *clone() const override {
			return new TestStepperMotor(*this);
		}
	private:
		StepsAction m_action;
	};
}

#endif // TEST_STEPPER_MOTOR_HPP