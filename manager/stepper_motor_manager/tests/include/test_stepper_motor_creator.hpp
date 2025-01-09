#ifndef	TEST_STEPPER_MOTOR_CREATOR_HPP
#define	TEST_STEPPER_MOTOR_CREATOR_HPP

#include <stdexcept>

#include "stepper_motor.hpp"
#include "stepper_motor_creator.hpp"
#include "test_stepper_motor.hpp"

namespace manager_tests {
    template <typename Tcreate_cfg>
	class TestStepperMotorCreator: public manager::StepperMotorCreator<Tcreate_cfg> {
	public:
        using StepAction = typename TestStepperMotor::StepAction;

        TestStepperMotorCreator(const StepAction& action): m_action(action) {
            if (!m_action) {
                throw std::invalid_argument("invalid action received");
            }
        }
        TestStepperMotorCreator(const TestStepperMotorCreator&) = delete;
        TestStepperMotorCreator& operator=(const TestStepperMotorCreator&) = delete;
		
        manager::StepperMotor *create(const Tcreate_cfg& create_cfg) const override {
            return new TestStepperMotor(m_action);
        }
    private:
        const StepAction m_action;
        bool m_is_enabled;
	};
}

#endif // TEST_STEPPER_MOTOR_CREATOR_HPP