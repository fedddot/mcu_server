#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

#include "manager.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"

namespace manager {
	class StepperMotorManager: public Manager<StepperMotorRequest, StepperMotorResponse> {
	public:
		using StepperMotorCreator = std::function<StepperMotor *(void)>;
		using DelayGenerator = std::function<void(const std::size_t& timeout_ms)>;

		StepperMotorManager(const StepperMotorCreator& stepper_ctor, const DelayGenerator& delay_generator);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest& request) override;
	private:
		const DelayGenerator m_delay_generator;

		std::unique_ptr<StepperMotor> m_motor;
	};

	inline StepperMotorManager::StepperMotorManager(const StepperMotorCreator& stepper_ctor, const DelayGenerator& delay_generator): m_motor(stepper_ctor()), m_delay_generator(delay_generator) {
		if (!m_motor) {
			throw std::runtime_error("failed to create StepperMotor instance");
		}
		if (!m_delay_generator) {
			throw std::invalid_argument("invalid delay_generator received");
		}
	}
	
	inline StepperMotorResponse StepperMotorManager::run(const StepperMotorRequest& request) {
		auto steps_to_go = request.steps_number;
		m_motor->set_state(State::ENABLED);
		while (steps_to_go) {
			m_motor->step(request.direction);
			m_delay_generator(request.step_duration_ms);
			--steps_to_go;
		}
		return StepperMotorResponse {
			StepperMotorResponse::ResultCode::OK,
			std::optional<State>(),
			std::optional<std::string>()
		};
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP