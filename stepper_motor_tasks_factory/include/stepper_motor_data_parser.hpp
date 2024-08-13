#ifndef	STEPPER_MOTOR_DATA_PARSER_HPP
#define	STEPPER_MOTOR_DATA_PARSER_HPP

#include "data.hpp"
#include "stepper_motor.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id, typename Ttask_type>
	class StepperMotorDataParser {
	public:
		using Duration = unsigned int;
		using StepsNumber = unsigned int;
		using Direction = typename mcu_platform::StepperMotor<Tgpio_id>::Direction;
		using Shoulders = typename mcu_platform::StepperMotor<Tgpio_id>::Shoulders;
		using States = typename mcu_platform::StepperMotor<Tgpio_id>::States;
		
		virtual ~StepperMotorDataParser() noexcept = 0;
		virtual StepperMotorDataParser *clone() const = 0;

		virtual Ttask_type parse_task_type(const mcu_server::Data& data) const = 0;
		virtual Tstepper_id parse_stepper_id(const mcu_server::Data& data) const = 0;
		virtual Duration parse_step_duration(const mcu_server::Data& data) const = 0;
		virtual StepsNumber parse_steps_number(const mcu_server::Data& data) const = 0;
		virtual Direction parse_steps_direction(const mcu_server::Data& data) const = 0;
		virtual Shoulders parse_shoulders(const mcu_server::Data& data) const = 0;
		virtual States parse_states(const mcu_server::Data& data) const = 0;
	};
}
#endif // STEPPER_MOTOR_DATA_PARSER_HPP