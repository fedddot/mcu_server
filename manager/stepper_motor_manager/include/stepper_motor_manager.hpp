#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include "clonable_manager.hpp"
#include "manager.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_request.hpp"
#include "stepper_motor_response.hpp"

namespace manager {
	class StepperMotorManager: public ClonableManager<StepperMotorRequest, StepperMotorResponse> {
	public:
		using Steppers = std::map<std::string, std::shared_ptr<StepperMotor>>;
		using SteppersCreator = std::function<Steppers()>;
		using DelayGenerator = std::function<void(const std::size_t& timeout_ms)>;

		StepperMotorManager(
			const SteppersCreator& steppers_ctor,
			const DelayGenerator& delay_generator
		);
		StepperMotorManager(const StepperMotorManager& other) = default;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest& request) override;
		Manager<StepperMotorRequest, StepperMotorResponse> *clone() const override;
	private:
		const DelayGenerator m_delay_generator;
		Steppers m_stepper_motors;
	};

	inline StepperMotorManager::StepperMotorManager(
		const SteppersCreator& steppers_ctor,
		const DelayGenerator& delay_generator
	): m_delay_generator(delay_generator) {
		if (!m_delay_generator) {
			throw std::invalid_argument("invalid delay_generator received");
		}
		if (!steppers_ctor) {
			throw std::invalid_argument("invalid steppers_ctor received");
		}
		m_stepper_motors = steppers_ctor();
		for (const auto& [motor_id, motor] : m_stepper_motors) {
			if (!motor) {
				throw std::invalid_argument("motor " + motor_id + " registered as nullptr");
			}
			motor->set_state(State::DISABLED);
		}
	}
	
	inline StepperMotorResponse StepperMotorManager::run(const StepperMotorRequest& request) {
		const auto iter = m_stepper_motors.find(request.motor_id);
		if (m_stepper_motors.end() == iter) {
			return StepperMotorResponse {
				StepperMotorResponse::ResultCode::NOT_FOUND,
				std::nullopt,
				std::string("motor " + request.motor_id + " not found")
			};
		}
		auto motor = (iter->second).get();
		if (!motor) {
			return StepperMotorResponse {
				StepperMotorResponse::ResultCode::EXCEPTION,
				std::nullopt,
				std::string("motor " + request.motor_id + " registered as nullptr")
			};
		}
		try {
			motor->set_state(State::ENABLED);
			auto steps_to_go = request.steps_number;
			while (steps_to_go) {
				motor->step(request.direction);
				m_delay_generator(request.step_duration_ms);
				--steps_to_go;
			}
			motor->set_state(State::DISABLED);
		} catch(...) {
			motor->set_state(State::DISABLED);
			throw;
		}
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