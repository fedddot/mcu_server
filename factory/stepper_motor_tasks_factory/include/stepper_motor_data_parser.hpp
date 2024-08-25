#ifndef	STEPPER_MOTOR_DATA_PARSER_HPP
#define	STEPPER_MOTOR_DATA_PARSER_HPP

#include "data.hpp"
#include "stepper_motor.hpp"
#include "steps_sequence_task.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id, typename Ttask_type>
	class StepperMotorDataParser {
	public:
		using Duration = unsigned int;
		using StepsNumber = unsigned int;
		using Direction = typename mcu_platform::StepperMotor<Tgpio_id>::Direction;
		using Shoulders = typename mcu_platform::StepperMotor<Tgpio_id>::Shoulders;
		using States = typename mcu_platform::StepperMotor<Tgpio_id>::States;
		using StepsSequence = typename StepsSequenceTask<Tstepper_id, Tgpio_id>::StepsSequence;
		
		virtual ~StepperMotorDataParser() noexcept = default;
		virtual StepperMotorDataParser *clone() const = 0;

		virtual Ttask_type parse_task_type(const server::Data& data) const = 0;
		virtual Tstepper_id parse_stepper_id(const server::Data& data) const = 0;
		virtual Duration parse_step_duration(const server::Data& data) const = 0;
		virtual StepsNumber parse_steps_number(const server::Data& data) const = 0;
		virtual Direction parse_steps_direction(const server::Data& data) const = 0;
		virtual Shoulders parse_shoulders(const server::Data& data) const = 0;
		virtual States parse_states(const server::Data& data) const = 0;
		virtual StepsSequence parse_steps_sequence(const server::Data& data) const = 0;
	};
}
#endif // STEPPER_MOTOR_DATA_PARSER_HPP