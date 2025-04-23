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
		using StepperMotorCreator = std::function<StepperMotor *(const std::string&)>;
		using DelayGenerator = std::function<void(const std::size_t& timeout_ms)>;

		StepperMotorManager(
			const StepperMotorCreator& stepper_ctor,
			const DelayGenerator& delay_generator,
			const std::vector<std::string>& motor_ids
		);
		StepperMotorManager(const StepperMotorManager& other) = default;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		StepperMotorResponse run(const StepperMotorRequest& request) override;
		Manager<StepperMotorRequest, StepperMotorResponse> *clone() const override;
	private:
		const DelayGenerator m_delay_generator;
		std::map<std::string, std::shared_ptr<StepperMotor>> m_stepper_motors;
	};

	inline StepperMotorManager::StepperMotorManager(
		const StepperMotorCreator& stepper_ctor,
		const DelayGenerator& delay_generator,
		const std::vector<std::string>& motor_ids
	): m_delay_generator(delay_generator) {
		if (!m_delay_generator) {
			throw std::invalid_argument("invalid delay_generator received");
		}
		if (!stepper_ctor) {
			throw std::invalid_argument("invalid stepper_ctor received");
		}
		for (const auto& motor_id : motor_ids) {
			if (m_stepper_motors.find(motor_id) != m_stepper_motors.end()) {
				throw std::invalid_argument("motor " + motor_id + " already exists");
			}
			m_stepper_motors[motor_id] = std::shared_ptr<StepperMotor>(stepper_ctor(motor_id));
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