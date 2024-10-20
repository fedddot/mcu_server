#ifndef	TEST_STEPPER_MOTOR_HPP
#define	TEST_STEPPER_MOTOR_HPP

#include <functional>
#include <stdexcept>

#include "stepper_motor.hpp"

namespace manager_uts {
	class TestStepperMotor: public manager::StepperMotor {
	public:
		using StepAction = std::function<void(const Direction&)>;
		TestStepperMotor(const StepAction& action): m_action(action), m_enabled(false) {
			if (!m_action) {
				throw std::invalid_argument("invalid action received");
			}
		}
		TestStepperMotor(const TestStepperMotor&) = delete;
		TestStepperMotor& operator=(const TestStepperMotor&) = delete;

		void enable() override {
			m_enabled = true;
		}
		virtual void disable() override {
			m_enabled = false;
		}
		virtual bool enabled() const override {
			return m_enabled;
		}
		virtual void step(const Direction& direction) override {
			m_action(direction);
		}
	private:
		StepAction m_action;
		bool m_enabled;
	};
}

#endif // TEST_STEPPER_MOTOR_HPP