#ifndef	TEST_STEPPER_MOTOR_DELAY_GENERATOR_HPP
#define	TEST_STEPPER_MOTOR_DELAY_GENERATOR_HPP

#include <chrono>
#include <thread>

#include "stepper_motor_delay_generator.hpp"

namespace manager_tests {
	class TestStepperMotorDelayGenerator: public manager::StepperMotorDelayGenerator {
	public:
        TestStepperMotorDelayGenerator() = default;
        TestStepperMotorDelayGenerator(const TestStepperMotorDelayGenerator&) = delete;
        TestStepperMotorDelayGenerator& operator=(const TestStepperMotorDelayGenerator&) = delete;

        void delay(const std::size_t& timeout_ms) const override {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
        }
	};
}

#endif // TEST_STEPPER_MOTOR_DELAY_GENERATOR_HPP