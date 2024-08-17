#ifndef	DEFAULT_STEPPER_MOTOR_DATA_PARSER_HPP
#define	DEFAULT_STEPPER_MOTOR_DATA_PARSER_HPP

#include <stdexcept>

#include "array.hpp"
#include "data.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_data_parser.hpp"
#include "stepper_motor_tasks_factory.hpp"

namespace mcu_factory {
	class DefaultStepperMotorDataParser: public StepperMotorDataParser<int, int, typename StepperMotorTasksFactory<int, int>::TaskType> {
	public:
		using TaskType = typename StepperMotorTasksFactory<int, int>::TaskType;
		DefaultStepperMotorDataParser() = default;
		DefaultStepperMotorDataParser(const DefaultStepperMotorDataParser& other) = default;
		DefaultStepperMotorDataParser& operator=(const DefaultStepperMotorDataParser& other) = default;

		DefaultStepperMotorDataParser *clone() const override;

		TaskType parse_task_type(const mcu_server::Data& data) const override;
		int parse_stepper_id(const mcu_server::Data& data) const override;
		Duration parse_step_duration(const mcu_server::Data& data) const override;
		StepsNumber parse_steps_number(const mcu_server::Data& data) const override;
		Direction parse_steps_direction(const mcu_server::Data& data) const override;
		Shoulders parse_shoulders(const mcu_server::Data& data) const override;
		States parse_states(const mcu_server::Data& data) const override;
		StepsSequence parse_steps_sequence(const mcu_server::Data& data) const override;
	private:
		using Shoulder = typename mcu_platform::StepperMotor<int>::Shoulder;
		using State = typename mcu_platform::StepperMotor<int>::State;
		static Shoulder str_to_shoulder(const std::string& shoulder_str);
	};

	inline DefaultStepperMotorDataParser *DefaultStepperMotorDataParser::clone() const {
		return new DefaultStepperMotorDataParser(*this);
	}

	inline typename DefaultStepperMotorDataParser::TaskType DefaultStepperMotorDataParser::parse_task_type(const mcu_server::Data& data) const {
		using namespace mcu_server;
		return static_cast<TaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
	}

	inline int DefaultStepperMotorDataParser::parse_stepper_id(const mcu_server::Data& data) const {
		using namespace mcu_server;
		return Data::cast<Integer>(Data::cast<Object>(data).access("stepper_id")).get();
	}

	inline typename DefaultStepperMotorDataParser::Duration DefaultStepperMotorDataParser::parse_step_duration(const mcu_server::Data& data) const {
		using namespace mcu_server;
		return static_cast<Duration>(Data::cast<Integer>(Data::cast<Object>(data).access("step_duration_ms")).get());
	}

	inline typename DefaultStepperMotorDataParser::StepsNumber DefaultStepperMotorDataParser::parse_steps_number(const mcu_server::Data& data) const {
		using namespace mcu_server;
		return static_cast<StepsNumber>(Data::cast<Integer>(Data::cast<Object>(data).access("steps_number")).get());
	}

	inline typename DefaultStepperMotorDataParser::Direction DefaultStepperMotorDataParser::parse_steps_direction(const mcu_server::Data& data) const {
		using namespace mcu_server;
		return static_cast<Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("direction")).get());
	}

	inline typename DefaultStepperMotorDataParser::Shoulders DefaultStepperMotorDataParser::parse_shoulders(const mcu_server::Data& data) const {
		Shoulders shoulders;
		using namespace mcu_server;
		Data::cast<Object>(Data::cast<Object>(data).access("shoulders")).for_each(
			[&shoulders](const std::string& shoulder_str, const Data& gpo_id) {
				shoulders.insert({str_to_shoulder(shoulder_str), Data::cast<Integer>(gpo_id).get()});
			}
		);
		return shoulders;
	}

	inline typename DefaultStepperMotorDataParser::States DefaultStepperMotorDataParser::parse_states(const mcu_server::Data& data) const {
		States states;
		using namespace mcu_server;
		using GpoState = mcu_platform::Gpo::State;
		Data::cast<Array>(Data::cast<Object>(data).access("states")).for_each(
			[&states](int state_index, const Data& state_data) {
				State state;
				Data::cast<Object>(state_data).for_each(
					[&state](const std::string& shoulder_str, const Data& gpo_state) {
						state.insert({str_to_shoulder(shoulder_str), static_cast<GpoState>(Data::cast<Integer>(gpo_state).get())});
					}
				);
				states.push_back(state);
			}
		);
		return states;
	}

	inline typename DefaultStepperMotorDataParser::Shoulder DefaultStepperMotorDataParser::str_to_shoulder(const std::string& shoulder_str) {
		const std::string prefix("IN");
		auto prefix_pos = shoulder_str.find(prefix);
		if (0 != prefix_pos) {
			throw std::invalid_argument("shoulder tag doesn't start with " + prefix);
		}
		auto shoulder_number = std::stoi(std::string(shoulder_str.begin() + prefix.size(), shoulder_str.end()));
		return static_cast<Shoulder>(shoulder_number);
	}

	inline typename DefaultStepperMotorDataParser::StepsSequence DefaultStepperMotorDataParser::parse_steps_sequence(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT IMPLEMENTED!");
	}
}
#endif // DEFAULT_STEPPER_MOTOR_DATA_PARSER_HPP