#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <functional>
#include <map>
#include <stdexcept>

#include "data.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "movement.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"

namespace manager {
	class LinearMovement: public Movement {
	public:
		using DelayFunction = std::function<void(const unsigned int)>;
		LinearMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const bool inverse_direction);
		void perform(const server::Data& cfg) override;
	private:
		Inventory<server::ResourceId, StepperMotor> *m_stepper_motor_inventory;
		DelayFunction m_delay;
		StepperMotor::Direction m_forward_direction;
		StepperMotor::Direction m_backward_direction;

		using MovementVector = std::map<server::ResourceId, int>;
		
		static unsigned int retrieve_steps_number(const server::Object& config);
		static unsigned int retrieve_step_duration(const server::Object& config);
		static MovementVector retrieve_coordinates(const server::Object& config);

		void run_step(std::map<server::ResourceId, int> *current_coordinates, const MovementVector& vector, const unsigned int step_number, const unsigned int steps_number, const unsigned int step_duration);

		void run_movement(const MovementVector& vector, const unsigned int steps_number, const unsigned int step_duration);
	};

	inline LinearMovement::LinearMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const bool inverse_direction): m_stepper_motor_inventory(stepper_motor_inventory), m_delay(delay) {
		if (!m_stepper_motor_inventory || !m_delay) {
			throw std::invalid_argument("invalid arguments received");
		}
		m_backward_direction = StepperMotor::Direction::CCW;
		m_backward_direction = StepperMotor::Direction::CW;
		if (inverse_direction) {
			m_backward_direction = StepperMotor::Direction::CW;
			m_backward_direction = StepperMotor::Direction::CCW;
		}
	}

	inline void LinearMovement::perform(const server::Data& cfg) {
		using namespace server;
		const auto& cfg_obj(Data::cast<server::Object>(cfg));
		run_movement(
			retrieve_coordinates(cfg_obj),
			retrieve_steps_number(cfg_obj),
			retrieve_step_duration(cfg_obj)
		);
	}

	inline unsigned int LinearMovement::retrieve_steps_number(const server::Object& config) {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(config.access("steps_number")).get());
	}

	inline unsigned int LinearMovement::retrieve_step_duration(const server::Object& config) {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(config.access("step_duration")).get());
	}
	
	inline typename LinearMovement::MovementVector LinearMovement::retrieve_coordinates(const server::Object& config) {
		using namespace server;
		MovementVector result;
		Data::cast<Object>(config.access("steps")).for_each(
			[&result](const std::string& motor_id, const Data& steps_num) {
				result.insert({ResourceId(motor_id), Data::cast<Integer>(steps_num).get()});
			}
		);
		return result;
	}

	inline void LinearMovement::run_step(std::map<server::ResourceId, int> *current_coordinates, const MovementVector& vector, const unsigned int step_number, const unsigned int steps_number, const unsigned int step_duration) {
		if (0 == steps_number) {
			throw server::ServerException(server::ResponseCode::BAD_REQUEST, "invalid steps number received");
		}
		for (const auto& [motor_id, steps_num]: vector) {
			const auto model_coordinate = static_cast<int>(step_number) * vector.at(motor_id) / static_cast<int>(steps_number);
			const auto current_coordinate = (*current_coordinates)[motor_id];
			if (current_coordinate > model_coordinate) {
				(m_stepper_motor_inventory->access(motor_id)).step(m_backward_direction);
				--(*current_coordinates)[motor_id];
				continue;
			} else if (current_coordinate < model_coordinate) {
				(m_stepper_motor_inventory->access(motor_id)).step(m_forward_direction);
				++(*current_coordinates)[motor_id];
				continue;
			}
		}
		m_delay(step_duration);			
	}

	inline void LinearMovement::run_movement(const MovementVector& vector, const unsigned int steps_number, const unsigned int step_duration) {
		std::map<server::ResourceId, int> current_coordinates;
		for (const auto& [motor_id, steps_num]: vector) {
			(void)steps_num;
			(m_stepper_motor_inventory->access(motor_id)).enable();
			current_coordinates[motor_id] = 0;
		}
		for (auto step_number = 0; step_number < steps_number; ++step_number) {
			run_step(&current_coordinates, vector, step_number, steps_number, step_duration);
		}
		for (const auto& [motor_id, steps_num]: vector) {
			(void)steps_num;
			(m_stepper_motor_inventory->access(motor_id)).disable();
		}			
	}
}

#endif // LINEAR_MOVEMENT_HPP