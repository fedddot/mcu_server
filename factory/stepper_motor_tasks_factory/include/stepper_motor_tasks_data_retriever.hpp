#ifndef	STEPPER_MOTOR_TASKS_DATA_RETRIEVER_HPP
#define	STEPPER_MOTOR_TASKS_DATA_RETRIEVER_HPP

#include "data.hpp"
#include "stepper_motor.hpp"

namespace mcu_factory {
	template <typename Ttask_type, typename Tgpio_id, typename Tstepper_id, typename Tsteps, typename Tsteps_sequence>
	class StepperMotorTasksDataRetriever {
	public:
		using Direction = typename mcu_platform::StepperMotor<Tgpio_id>::Direction;
		using States = typename mcu_platform::StepperMotor<Tgpio_id>::States;
		using Shoulders = typename mcu_platform::StepperMotor<Tgpio_id>::Shoulders;
		
		virtual ~StepperMotorTasksDataRetriever() noexcept = default;
		
		virtual bool is_stepper_motor_task_creatable(const server::Data& data) const = 0;
		virtual Ttask_type retrieve_task_type(const server::Data& data) const = 0;
		virtual Tgpio_id retrieve_gpio_id(const server::Data& data) const = 0;
		virtual Tstepper_id retrieve_stepper_id(const server::Data& data) const = 0;
		virtual Direction retrieve_dir(const server::Data& data) const = 0;
		virtual unsigned int retrieve_step_duration(const server::Data& data) const = 0;
		virtual unsigned int retrieve_steps_number(const server::Data& data) const = 0;
		virtual Shoulders retrieve_shoulders(const server::Data& data) const = 0;
		virtual States retrieve_states(const server::Data& data) const = 0;
		virtual Tsteps retrieve_steps(const server::Data& data) const = 0;
		virtual Tsteps_sequence retrieve_steps_sequence(const server::Data& data) const = 0;
		virtual StepperMotorTasksDataRetriever *clone() const = 0;
	};
}
#endif // STEPPER_MOTOR_TASKS_DATA_RETRIEVER_HPP