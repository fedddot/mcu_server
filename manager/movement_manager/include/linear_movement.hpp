#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "integer.hpp"
#include "linear_movement_model.hpp"
#include "data.hpp"
#include "double.hpp"
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
		using DelayFunction = std::function<void(const double&)>;
		using Axis = typename Vector<double>::Axis;
		using AxesAssignment = std::map<Axis, server::ResourceId>;
		
		LinearMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const AxesAssignment& axes_assignment, const unsigned int steps_per_length);
		void perform(const manager::Data& cfg) override;
		Type type() const override;
	private:
		Inventory<server::ResourceId, StepperMotor> *m_stepper_motor_inventory;
		DelayFunction m_delay;
		AxesAssignment m_axes_assignment;
		unsigned int m_steps_per_length;
		double m_step_length;
		StepperMotor::Direction m_forward_direction;
		StepperMotor::Direction m_backward_direction;

		double retrieve_feed(const server::Object& config) const;
		static Vector<double> retrieve_vector(const server::Object& config, const std::string& vector_name);
		
		static Axis str_to_axis(const std::string& axis_str);

		void process_deviation(Vector<double> *prosition, const Vector<double>& deviation);
		void enable_motors();
		void disable_motors();
	};

	inline LinearMovement::LinearMovement(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const DelayFunction& delay, const AxesAssignment& axes_assignment, const unsigned int steps_per_length): m_stepper_motor_inventory(stepper_motor_inventory), m_delay(delay), m_axes_assignment(axes_assignment), m_steps_per_length(steps_per_length), m_forward_direction(StepperMotor::Direction::CCW), m_backward_direction(StepperMotor::Direction::CW) {
		if (!m_stepper_motor_inventory || !m_delay || !m_steps_per_length) {
			throw std::invalid_argument("invalid arguments received");
		}
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			if (m_axes_assignment.end() == m_axes_assignment.find(axis)) {
				throw std::invalid_argument("invalid axes assignment - missing axes");
			}
		}
		m_step_length = static_cast<double>(1) / m_steps_per_length;
	}

	inline void LinearMovement::perform(const manager::Data& cfg) {
		using namespace server;
		const auto& cfg_obj(Data::cast<server::Object>(cfg));
		const auto target_vector(retrieve_vector(cfg_obj, "target"));
		const auto feed(retrieve_feed(cfg_obj));
		
		LinearMovementModel model(
			target_vector,
			feed,
			m_steps_per_length
		);
		const auto dt(model.dt());
		enable_motors();
		Vector<double> position(0, 0, 0);
		while (!model.finished()) {
			const auto model_position(model.evaluate());
			const auto deviation(model_position - position);
			process_deviation(&position, deviation);
			m_delay(dt);
		}
		disable_motors();
	}
	
	inline typename LinearMovement::Type LinearMovement::type() const {
		return Type::LINEAR;
	}

	inline double LinearMovement::retrieve_feed(const server::Object& config) const {
		using namespace server;
		double feed(0);
		const auto& feed_data(config.access("feed"));
		switch (feed_data.type()) {
		case Data::Type::DOUBLE:
			feed = Data::cast<Double>(feed_data).get();
			break;
		case Data::Type::INT:
			feed = static_cast<double>(Data::cast<Integer>(feed_data).get());
			break;
		default:
			throw ServerException(ResponseCode::BAD_REQUEST, "failed to reitieve feed - invalid data type");
		}
		if (0 == feed) {
			throw ServerException(ResponseCode::BAD_REQUEST, "feed is zero");
		}
		return feed;
	}

	inline Vector<double> LinearMovement::retrieve_vector(const server::Object& config, const std::string& name) {
		using namespace server;
		Vector<double> result(0, 0, 0);
		Data::cast<Object>(config.access(name)).for_each(
			[&result](const std::string& axis_tag, const Data& value) {
				double projection(0);
				switch (value.type()) {
				case Data::Type::DOUBLE:
					projection = Data::cast<Double>(value).get();
					break;
				case Data::Type::INT:
					projection = static_cast<double>(Data::cast<Integer>(value).get());
					break;
				default:
					throw ServerException(ResponseCode::BAD_REQUEST, "failed to reitieve projection - invalid data type");
				}
				result.set_projection(str_to_axis(axis_tag), projection);
			}
		);
		return result;
	}
	
	inline typename LinearMovement::Axis LinearMovement::str_to_axis(const std::string& axis_str) {
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

	inline void LinearMovement::process_deviation(Vector<double> *position, const Vector<double>& deviation) {
		using Axis = typename Vector<double>::Axis;
		for (const auto& axis: {Axis::X, Axis::Y, Axis::Z}) {
			if (deviation.projection(axis) > m_step_length) {
				(m_stepper_motor_inventory->access(m_axes_assignment.at(axis))).step(m_forward_direction);
				position->set_projection(axis, position->projection(axis) + m_step_length);
			} else if (deviation.projection(axis) < -m_step_length) {
				(m_stepper_motor_inventory->access(m_axes_assignment.at(axis))).step(m_backward_direction);
				position->set_projection(axis, position->projection(axis) - m_step_length);
			}
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