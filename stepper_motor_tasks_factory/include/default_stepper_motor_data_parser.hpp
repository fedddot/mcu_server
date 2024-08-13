#ifndef	DEFAULT_STEPPER_MOTOR_DATA_PARSER_HPP
#define	DEFAULT_STEPPER_MOTOR_DATA_PARSER_HPP

#include <stdexcept>

#include "stepper_motor_data_parser.hpp"
#include "stepper_motor_tasks_factory.hpp"

namespace mcu_factory {
	class DefaultStepperMotorDataParser: StepperMotorDataParser<int, int, typename StepperMotorTasksFactory<int, int>::TaskType> {
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
	};

	inline DefaultStepperMotorDataParser *DefaultStepperMotorDataParser::clone() const {
		return new DefaultStepperMotorDataParser(*this);
	}

	inline typename DefaultStepperMotorDataParser::TaskType DefaultStepperMotorDataParser::parse_task_type(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT_IMPLEMENTED");
	}

	inline int DefaultStepperMotorDataParser::parse_stepper_id(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT_IMPLEMENTED");
	}

	inline typename DefaultStepperMotorDataParser::Duration DefaultStepperMotorDataParser::parse_step_duration(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT_IMPLEMENTED");
	}

	inline typename DefaultStepperMotorDataParser::StepsNumber DefaultStepperMotorDataParser::parse_steps_number(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT_IMPLEMENTED");
	}

	inline typename DefaultStepperMotorDataParser::Direction DefaultStepperMotorDataParser::parse_steps_direction(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT_IMPLEMENTED");
	}

	inline typename DefaultStepperMotorDataParser::Shoulders DefaultStepperMotorDataParser::parse_shoulders(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT_IMPLEMENTED");
	}

	inline typename DefaultStepperMotorDataParser::States DefaultStepperMotorDataParser::parse_states(const mcu_server::Data& data) const {
		throw std::runtime_error("NOT_IMPLEMENTED");
	}
}
#endif // DEFAULT_STEPPER_MOTOR_DATA_PARSER_HPP