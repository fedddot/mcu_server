#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <cmath>
#include <exception>
#include <functional>
#include <map>
#include <stdexcept>
#include <vector>

#include "data.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "movement.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "vector.hpp"

namespace manager {
	class LinearMovement: public Movement {
	public:
		using TimeUnit = unsigned int;
		using DelayFunction = std::function<void(const TimeUnit&)>;
		using Axis = typename Vector<int>::Axis;
		using AxesAssignment = std::map<Axis, server::ResourceId>;
		
		LinearMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const AxesAssignment& axes_assignment, const unsigned int time_multiplicator);
		void perform(const server::Data& cfg) override;
		Type type() const override;
	private:
		Inventory<server::ResourceId, StepperMotor> *m_stepper_motor_inventory;
		DelayFunction m_delay;
		AxesAssignment m_axes_assignment;
		unsigned int m_time_multiplicator;
		StepperMotor::Direction m_forward_direction;
		StepperMotor::Direction m_backward_direction;

		using MovementVector = std::map<Axis, int>;

		static unsigned int retrieve_feed(const server::Object& config);
		static MovementVector retrieve_vector(const server::Object& config);
		static unsigned int principal_projection_length(const MovementVector& vector);
		static unsigned long vector_length(const MovementVector& vector);

		void run_movement(const MovementVector& vector, const unsigned int feed);
		void enable_motors();
		void disable_motors();
	};

	inline LinearMovement::LinearMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const AxesAssignment& axes_assignment, const unsigned int time_multiplicator): m_stepper_motor_inventory(stepper_motor_inventory), m_delay(delay), m_axes_assignment(axes_assignment), m_forward_direction(StepperMotor::Direction::CCW), m_backward_direction(StepperMotor::Direction::CW), m_time_multiplicator(time_multiplicator) {
		if (!m_stepper_motor_inventory || !m_delay) {
			throw std::invalid_argument("invalid arguments received");
		}
		if (1UL > m_time_multiplicator) {
			throw std::invalid_argument("invalid time multiplicator received");
		}
		for (const auto& axis: std::vector<Axis>{Axis::X, Axis::Y, Axis::Z}) {
			if (m_axes_assignment.end() == m_axes_assignment.find(axis)) {
				throw std::invalid_argument("invalid axes assignment - missing axes");
			}
		}
	}

	inline void LinearMovement::perform(const server::Data& cfg) {
		using namespace server;
		const auto& cfg_obj(Data::cast<server::Object>(cfg));
		const auto vector(retrieve_vector(cfg_obj));
		const auto feed(retrieve_feed(cfg_obj));
	
		try {
			enable_motors();
			run_movement(vector, feed);
			disable_motors();
		} catch (const std::exception& e) {
			disable_motors();
			throw ServerException(ResponseCode::UNSPECIFIED, std::string(e.what()));
		}
	}
	
	inline typename LinearMovement::Type LinearMovement::type() const {
		return Type::LINEAR;
	}

	inline unsigned int LinearMovement::retrieve_feed(const server::Object& config) {
		using namespace server;
		return static_cast<unsigned int>(Data::cast<Integer>(config.access("feed")).get());
	}

	inline typename LinearMovement::MovementVector LinearMovement::retrieve_vector(const server::Object& config) {
		using namespace server;
		MovementVector result;
		auto str_to_axis = [](const std::string& axis_tag) {
			if ("x" == axis_tag) {
				return Axis::X;
			} else if ("y" == axis_tag) {
				return Axis::Y;
			} else if ("z" == axis_tag) {
				return Axis::Z;
			}
			throw std::invalid_argument("failed to retrieve axis");
		};
		Data::cast<Object>(config.access("vector")).for_each(
			[&result, str_to_axis](const std::string& axis_tag, const Data& steps_num) {
				result.insert({str_to_axis(axis_tag), Data::cast<Integer>(steps_num).get()});
			}
		);
		return result;
	}
	
	inline unsigned int LinearMovement::principal_projection_length(const MovementVector& vector) {
		unsigned int result(0);
		for (const auto& [axis, coordinate]: vector) {
			const auto projection(static_cast<unsigned int>(abs(coordinate)));
			if (projection > result) {
				result = projection;
			}
		}
		return result;
	}
	
	inline unsigned long LinearMovement::vector_length(const MovementVector& vector) {
		unsigned long result(0);
		for (const auto& [axis, coordinate]: vector) {
			result += coordinate * coordinate;
		}
		return static_cast<unsigned long>(std::sqrt(result));
	}

	inline void LinearMovement::run_movement(const MovementVector& vector, const unsigned int feed) {
		if (0 == feed) {
			throw std::invalid_argument("invalid feed received - can't be a zero");
		}
		const auto principal_length(principal_projection_length(vector));
		if (0 == principal_length) {
			return;
		}
		const auto length(vector_length(vector));
		const auto step_duration = (m_time_multiplicator * length) / (feed * principal_length);
		MovementVector position {
			{Axis::X, 0},
			{Axis::Y, 0},
			{Axis::Z, 0}
		};
		for (auto step_number = 0; step_number < principal_length; ++step_number) {
			for (const auto& [axis, coordinate]: vector) {
				const auto model_coordinate = static_cast<long>(step_number) * vector.at(axis) / static_cast<long>(principal_length);
				const auto current_coordinate = position[axis];
				if (current_coordinate > model_coordinate) {
					(m_stepper_motor_inventory->access(m_axes_assignment.at(axis))).step(m_backward_direction);
					--position[axis];
					continue;
				} else if (current_coordinate < model_coordinate) {
					(m_stepper_motor_inventory->access(m_axes_assignment.at(axis))).step(m_forward_direction);
					++position[axis];
					continue;
				}
			}
			m_delay(step_duration);	
		}
	}

	inline void LinearMovement::enable_motors() {
		for (const auto& [axis, motor_id]: m_axes_assignment) {
			(m_stepper_motor_inventory->access(motor_id)).enable();
		}
	}
	
	inline void LinearMovement::disable_motors() {
		for (const auto& [axis, motor_id]: m_axes_assignment) {
			(m_stepper_motor_inventory->access(motor_id)).disable();
		}
	}
}

#endif // LINEAR_MOVEMENT_HPP