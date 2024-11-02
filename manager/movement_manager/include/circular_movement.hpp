#ifndef	CIRCULAR_MOVEMENT_HPP
#define	CIRCULAR_MOVEMENT_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "circular_movement_model.hpp"
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
	class CircularMovement: public Movement {
	public:
		using TimeUnit = unsigned int;
		using Feed = float;
		using Direction = typename CircularMovementModel::Direction;
		using DelayFunction = std::function<void(const TimeUnit&)>;
		using Axis = typename Vector<int>::Axis;
		using AxesAssignment = std::map<Axis, server::ResourceId>;
		
		CircularMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const AxesAssignment& axes_assignment, const unsigned int time_multiplicator);
		void perform(const server::Data& cfg) override;
		Type type() const override;
	private:
		Inventory<server::ResourceId, StepperMotor> *m_stepper_motor_inventory;
		DelayFunction m_delay;
		AxesAssignment m_axes_assignment;
		unsigned int m_time_multiplicator;
		StepperMotor::Direction m_forward_direction;
		StepperMotor::Direction m_backward_direction;
		TimeUnit m_hold_time;

		Feed retrieve_feed(const server::Object& config) const;
		static Vector<int> retrieve_vector(const server::Object& config, const std::string& vector_name);
		static Direction retrieve_direction(const server::Object& config);
		static TimeUnit retrieve_time_resolution(const server::Object& config);
		
		static Axis str_to_axis(const std::string& axis_str);

		void process_deviation(Vector<int> *prosition, const Vector<int>& deviation);
		void enable_motors();
		void disable_motors();
	};

	inline CircularMovement::CircularMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const AxesAssignment& axes_assignment, const unsigned int time_multiplicator): m_stepper_motor_inventory(stepper_motor_inventory), m_delay(delay), m_axes_assignment(axes_assignment), m_forward_direction(StepperMotor::Direction::CCW), m_backward_direction(StepperMotor::Direction::CW), m_time_multiplicator(time_multiplicator) {
		if (!m_stepper_motor_inventory || !m_delay) {
			throw std::invalid_argument("invalid arguments received");
		}
		if (1UL > m_time_multiplicator) {
			throw std::invalid_argument("invalid time multiplicator received");
		}
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			if (m_axes_assignment.end() == m_axes_assignment.find(axis)) {
				throw std::invalid_argument("invalid axes assignment - missing axes");
			}
		}
	}

	inline void CircularMovement::perform(const server::Data& cfg) {
		using namespace server;
		const auto& cfg_obj(Data::cast<server::Object>(cfg));
		const auto rotation_vector(retrieve_vector(cfg_obj, "rotation_center"));
		const auto target_vector(retrieve_vector(cfg_obj, "target"));
		const auto feed(retrieve_feed(cfg_obj));
		const auto direction(retrieve_direction(cfg_obj));
		const int dl(1);
		const auto dt(static_cast<double>(dl) / feed);
		
		const CircularMovementModel model(
			target_vector,
			rotation_vector,
			direction,
			feed
		);
		const auto tmax(model.tmax());
		enable_motors();
		Vector<int> position(0, 0, 0);
		for (double t = 0; t < tmax; t += dt) {
			const auto model_position(model.evaluate(t));
			const auto deviation(model_position - position);
			process_deviation(&position, deviation);
			m_delay(dt * m_time_multiplicator);
		}
		disable_motors();
	}
	
	inline typename CircularMovement::Type CircularMovement::type() const {
		return Type::CIRCULAR;
	}

	inline typename CircularMovement::Feed CircularMovement::retrieve_feed(const server::Object& config) const {
		using namespace server;
		const auto feed = static_cast<Feed>(Data::cast<Integer>(config.access("feed")).get());
		if (0 == feed) {
			throw ServerException(ResponseCode::BAD_REQUEST, "feed is zero");
		}
		return feed;
	}

	inline typename CircularMovement::Direction CircularMovement::retrieve_direction(const server::Object& config) {
		using namespace server;
		return static_cast<Direction>(Data::cast<Integer>(config.access("direction")).get());
	}
	
	inline Vector<int> CircularMovement::retrieve_vector(const server::Object& config, const std::string& name) {
		using namespace server;
		Vector<int> result(0, 0, 0);
		Data::cast<Object>(config.access(name)).for_each(
			[&result](const std::string& axis_tag, const Data& value) {
				result.set_projection(str_to_axis(axis_tag), Data::cast<Integer>(value).get());
			}
		);
		return result;
	}
	
	inline typename CircularMovement::Axis CircularMovement::str_to_axis(const std::string& axis_str) {
		static const std::map<std::string, Axis> mapping {
			{"x", Axis::X},
			{"y", Axis::Y},
			{"z", Axis::Z}
		};
		const auto iter = mapping.find(axis_str);
		if (mapping.end() == iter) {
			throw std::invalid_argument("invalid axis tag received: " + axis_str);
		}
		return iter->second;
	}

	inline void CircularMovement::process_deviation(Vector<int> *position, const Vector<int>& deviation) {
		using Axis = typename Vector<int>::Axis;
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			if (deviation.projection(axis) > 0) {
				(m_stepper_motor_inventory->access(m_axes_assignment.at(axis))).step(m_forward_direction);
				position->set_projection(axis, position->projection(axis) + 1);
			} else if (deviation.projection(axis) < 0) {
				(m_stepper_motor_inventory->access(m_axes_assignment.at(axis))).step(m_backward_direction);
				position->set_projection(axis, position->projection(axis) - 1);
			}
		}
	}

	inline void CircularMovement::enable_motors() {
		for (const auto& [axis, motor_id]: m_axes_assignment) {
			(m_stepper_motor_inventory->access(motor_id)).enable();
		}
	}
	
	inline void CircularMovement::disable_motors() {
		for (const auto& [axis, motor_id]: m_axes_assignment) {
			(m_stepper_motor_inventory->access(motor_id)).disable();
		}
	}
}

#endif // CIRCULAR_MOVEMENT_HPP