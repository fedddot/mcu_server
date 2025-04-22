#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>

#include "clonable_manager.hpp"
#include "manager.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"

namespace manager {
	class StepperMotorManager: public ClonableManager<StepperMotorRequest, StepperMotorResponse> {
	public:
		using StepperMotorCreator = std::function<StepperMotor *(void)>;
		using DelayGenerator = std::function<void(const std::size_t& timeout_ms)>;

		StepperMotorManager(const StepperMotorCreator& stepper_ctor, const DelayGenerator& delay_generator);
		StepperMotorManager(const StepperMotorManager& other) = default;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest& request) override;
		Manager<StepperMotorRequest, StepperMotorResponse> *clone() const override;
	private:
		const DelayGenerator m_delay_generator;
		std::shared_ptr<StepperMotor> m_motor;
	};

	inline StepperMotorManager::StepperMotorManager(const StepperMotorCreator& stepper_ctor, const DelayGenerator& delay_generator): m_delay_generator(delay_generator) {
		if (!m_delay_generator) {
			throw std::invalid_argument("invalid delay_generator received");
		}
	}
	
	inline StepperMotorResponse StepperMotorManager::run(const StepperMotorRequest& request) {
		if (!m_motor) {
			m_motor.reset(m_stepper_ctor());
		}
		auto steps_to_go = request.steps_number;
		m_motor->set_state(State::ENABLED);
		while (steps_to_go) {
			m_motor->step(request.direction);
			m_delay_generator(request.step_duration_ms);
			--steps_to_go;
		}
		m_motor->set_state(State::DISABLED);
		return StepperMotorResponse {
			StepperMotorResponse::ResultCode::OK,
			std::nullopt,
			std::nullopt
		};
	}

	inline Manager<StepperMotorRequest, StepperMotorResponse> *StepperMotorManager::clone() const {
		return new StepperMotorManager(*this);
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP