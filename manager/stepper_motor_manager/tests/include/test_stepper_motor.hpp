#ifndef	TEST_STEPPER_MOTOR_HPP
#define	TEST_STEPPER_MOTOR_HPP

#include <functional>
#include <stdexcept>

#include "stepper_motor.hpp"
#include "stepper_motor_data.hpp"

namespace manager_tests {
	class TestStepperMotor: public manager::StepperMotor {
	public:
        using StepAction = std::function<void(const manager::Direction&)>;

        TestStepperMotor(const StepAction& action): m_action(action), m_state(manager::State::DISABLED) {
            if (!m_action) {
                throw std::invalid_argument("invalid action received");
            }
        }
        TestStepperMotor(const TestStepperMotor&) = delete;
        TestStepperMotor& operator=(const TestStepperMotor&) = delete;
		
        void set_state(const manager::State& state) override {
            m_state = state;
        }

		manager::State state() const override {
            return m_state;
        }

		void step(const manager::Direction& direction) override {
            m_action(direction);
        }
    private:
        const StepAction m_action;
        manager::State m_state;
	};
}

#endif // TEST_STEPPER_MOTOR_HPP