#ifndef	STEPPER_MOTOR_TASKS_FACTORY_HPP
#define	STEPPER_MOTOR_TASKS_FACTORY_HPP

#include <memory>
#include <stdexcept>

#include "create_stepper_motor_task.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "parser.hpp"
#include "stepper_motor.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id>
	class StepperMotorTasksFactory: public mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)> {
	public:
		enum class TaskType: int {
			CREATE_STEPPER_MOTOR,
			DELETE_STEPPER_MOTOR,
			STEPS
		};
		using StepperMotorInventory = mcu_platform::Inventory<Tstepper_id, mcu_platform::StepperMotor<Tgpio_id>>;
		using TaskTypeParser = mcu_server::Parser<TaskType(const mcu_server::Data&)>;
		using StepperIdParser = mcu_server::Parser<Tstepper_id(const mcu_server::Data&)>;
		using StatesParser = mcu_server::Parser<typename mcu_platform::StepperMotor<Tgpio_id>::States(const mcu_server::Data&)>;
		using ShouldersParser = mcu_server::Parser<typename mcu_platform::StepperMotor<Tgpio_id>::Shoulders(const mcu_server::Data&)>;
		
		StepperMotorTasksFactory(
			StepperMotorInventory *inventory,
			const TaskTypeParser& task_type_parser,
			const StepperIdParser& stepper_id_parser,
			const StatesParser& states_parser,
			const ShouldersParser& shoulders_parser
		);
		StepperMotorTasksFactory(const StepperMotorTasksFactory& other);
		StepperMotorTasksFactory& operator=(const StepperMotorTasksFactory& other) = delete;

		mcu_server::Task<mcu_server::Data *(void)> *create(const mcu_server::Data& data) const override;
		mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)> *clone() const override;
	private:
		StepperMotorInventory *m_inventory;
		std::unique_ptr<TaskTypeParser> m_task_type_parser;
		std::unique_ptr<StepperIdParser> m_stepper_id_parser;
		std::unique_ptr<StatesParser> m_states_parser;
		std::unique_ptr<ShouldersParser> m_shoulders_parser;
		
		using TaskCtor = mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)>;
		const std::map<TaskType, std::unique_ptr<TaskCtor>> m_ctors;
	};

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepperMotorTasksFactory(
		StepperMotorInventory *inventory,
		const TaskTypeParser& task_type_parser,
		const StepperIdParser& stepper_id_parser,
		const StatesParser& states_parser,
		const ShouldersParser& shoulders_parser
	):
		m_inventory(inventory),
		m_task_type_parser(task_type_parser.clone()),
		m_stepper_id_parser(stepper_id_parser.clone()),
		m_states_parser(states_parser.clone()),
		m_shoulders_parser(shoulders_parser.clone()) {
		
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepperMotorTasksFactory(const StepperMotorTasksFactory& other):
		m_inventory(other.m_inventory),
		m_task_type_parser(other.m_task_type_parser->clone()),
		m_stepper_id_parser(other.m_stepper_id_parser->clone()),
		m_states_parser(other.m_states_parser->clone()),
		m_shoulders_parser(other.m_shoulders_parser->clone()) {
		
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline mcu_server::Task<mcu_server::Data *(void)> *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::create(const mcu_server::Data& data) const {
		auto task_type = m_task_type_parser->parse(data);
		switch (task_type) {
		case TaskType::CREATE_STEPPER_MOTOR:
			return new CreateStepperMotorTask<Tstepper_id>();
		// case TaskType::DELETE_STEPPER_MOTOR:
		// 	return new DeleteStepperMotorTask();
		// case TaskType::STEPS:
		// 	return new StepsTask();
		default:
			throw std::invalid_argument("invalid task type received");
		}
	}
	
	template <typename Tstepper_id, typename Tgpio_id>
	inline mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)> *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::clone() const {
		return new StepperMotorTasksFactory(*this);
	}
}
#endif // STEPPER_MOTOR_TASKS_FACTORY_HPP