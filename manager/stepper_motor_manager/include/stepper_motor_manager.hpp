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
		if (!stepper_ctor) {
			throw std::invalid_argument("invalid stepper_ctor received");
		}
		m_motor = std::shared_ptr<StepperMotor>(stepper_ctor());
	}
	
	inline StepperMotorResponse StepperMotorManager::run(const StepperMotorRequest& request) {
		auto response = StepperMotorResponse();
		try {
			m_motor->set_state(State::ENABLED);
			auto steps_to_go = request.steps_number;
			while (steps_to_go) {
				m_motor->step(request.direction);
				m_delay_generator(request.step_duration_ms);
				--steps_to_go;
			}
			response = StepperMotorResponse {
				StepperMotorResponse::ResultCode::OK,
				std::nullopt,
				std::nullopt
			};
		} catch (const std::exception& e) {
			response = StepperMotorResponse {
				StepperMotorResponse::ResultCode::EXCEPTION,
				std::nullopt,
				std::make_optional<std::string>(e.what())
			};
		}
		m_motor->set_state(State::DISABLED);
		return response;
	}

	inline Manager<StepperMotorRequest, StepperMotorResponse> *StepperMotorManager::clone() const {
		return new StepperMotorManager(*this);
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP