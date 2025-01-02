#ifndef	TEST_STEPPER_MOTOR_HPP
#define	TEST_STEPPER_MOTOR_HPP

#include <functional>
#include <stdexcept>

#include "stepper_motor.hpp"
#include "stepper_motor_data.hpp"

namespace manager_tests {
	class TestStepperMotor: public manager::StepperMotor {
	public:
        using StepAction = std::function<void(const manager::StepperMotorDirection&)>;

        TestStepperMotor(const StepAction& action): m_action(action), m_is_enabled(false) {
            if (!m_action) {
                throw std::invalid_argument("invalid action received");
            }
        }
        TestStepperMotor(const TestStepperMotor&) = delete;
        TestStepperMotor& operator=(const TestStepperMotor&) = delete;
		
        void enable() override {
            m_is_enabled = true;
        }
		void disable() override {
            m_is_enabled = false;
        }
		bool enabled() const override {
            return m_is_enabled;
        }
		void step(const manager::StepperMotorDirection& direction) override {
            m_action(direction);
        }
    private:
        const StepAction m_action;
        bool m_is_enabled;
	};
}

#endif // TEST_STEPPER_MOTOR_HPP