#ifndef	TEST_STEPPER_MOTOR_HPP
#define	TEST_STEPPER_MOTOR_HPP

#include <functional>
#include <stdexcept>

#include "stepper_motor.hpp"

namespace manager_tests {
	class TestStepperMotor: public manager::StepperMotor {
	public:
        using StepAction = std::function<void(const Direction&)>;

        TestStepperMotor(const StepAction& action): m_action(action), m_state(State::DISABLED) {
            if (!m_action) {
                throw std::invalid_argument("invalid action received");
            }
        }
        TestStepperMotor(const TestStepperMotor&) = delete;
        TestStepperMotor& operator=(const TestStepperMotor&) = delete;
		
        void set_state(const State& state) override {
            m_state = state;
        }

		State state() const override {
            return m_state;
        }

		void step(const Direction& direction) override {
            m_action(direction);
        }
    private:
        const StepAction m_action;
        State m_state;
	};
}

#endif // TEST_STEPPER_MOTOR_HPP