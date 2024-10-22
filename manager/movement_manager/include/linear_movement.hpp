#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <vector>

#include "array.hpp"
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
		using MovementVectors = std::vector<MovementVector>;
		
		static unsigned int retrieve_step_duration(const server::Object& config);
		static MovementVectors retrieve_vectors(const server::Object& config);

		static MovementVector init_vector(const MovementVector& other);
		static int principal_projection(const MovementVector& vector);
		void run_movement(const MovementVector& vector, const unsigned int step_duration);
	};

	inline LinearMovement::LinearMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const bool inverse_direction): m_stepper_motor_inventory(stepper_motor_inventory), m_delay(delay) {
		if (!m_stepper_motor_inventory || !m_delay) {
			throw std::invalid_argument("invalid arguments received");
		}
		m_backward_direction = StepperMotor::Direction::CCW;
		m_forward_direction = StepperMotor::Direction::CW;
		if (inverse_direction) {
			m_backward_direction = StepperMotor::Direction::CW;
			m_forward_direction = StepperMotor::Direction::CCW;
		}
	}

	inline void LinearMovement::perform(const server::Data& cfg) {
		using namespace server;
		const auto& cfg_obj(Data::cast<server::Object>(cfg));
		const auto movement_vectors(retrieve_vectors(cfg_obj));
		const auto step_duration(retrieve_step_duration(cfg_obj));
		for (const auto& vector: movement_vectors) {
			run_movement(vector, step_duration);
		}
	}

	inline unsigned int LinearMovement::retrieve_step_duration(const server::Object& config) {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(config.access("step_duration")).get());
	}

	inline typename LinearMovement::MovementVectors LinearMovement::retrieve_vectors(const server::Object& config) {
		using namespace server;
		MovementVectors result;
		Data::cast<Array>(config.access("steps")).for_each(
			[&result](int, const Data& steps_data) {
				MovementVector vector;
				Data::cast<Object>(steps_data).for_each(
					[&vector](const std::string& motor_id, const Data& steps_num) {
						vector.insert({ResourceId(motor_id), Data::cast<Integer>(steps_num).get()});
					}
				);
				result.push_back(vector);
			}
		);
		return result;
	}

	inline typename LinearMovement::MovementVector LinearMovement::init_vector(const MovementVector& other) {
		MovementVector result;
		for (const auto& [axis, coordinate]: other) {
			(void)coordinate;
			result[axis] = 0;
		}
		return result;
	}
	
	inline int LinearMovement::principal_projection(const MovementVector& vector) {
		int result(0);
		for (const auto& [axis, coordinate]: vector) {
			const auto projection(abs(coordinate));
			if (projection > result) {
				result = projection;
			}
		}
		return result;
	}

	inline void LinearMovement::run_movement(const MovementVector& vector, const unsigned int step_duration) {
		if (step_duration == 0) {
			throw server::ServerException(server::ResponseCode::BAD_REQUEST, "invalid step_duration received");
		}
		const auto principal_length(principal_projection(vector));
		if (0 == principal_length) {
			return;
		}
		auto position(init_vector(vector));
		for (auto step_number = 0; step_number < principal_length; ++step_number) {
			for (const auto& [axis, coordinate]: vector) {
				const auto model_coordinate = step_number * vector.at(axis) / principal_length;
				const auto current_coordinate = position[axis];
				if (current_coordinate > model_coordinate) {
					(m_stepper_motor_inventory->access(axis)).step(m_backward_direction);
					--position[axis];
					continue;
				} else if (current_coordinate < model_coordinate) {
					(m_stepper_motor_inventory->access(axis)).step(m_forward_direction);
					++position[axis];
					continue;
				}
			}
			m_delay(step_duration);	
		}
		for (const auto& [motor_id, steps_num]: vector) {
			(void)steps_num;
			(m_stepper_motor_inventory->access(motor_id)).disable();
		}			
	}
}

#endif // LINEAR_MOVEMENT_HPP