#ifndef	STEPPER_MOTOR_TASKS_FACTORY_HPP
#define	STEPPER_MOTOR_TASKS_FACTORY_HPP

#include <memory>
#include <stdexcept>

#include "create_stepper_motor_task.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "delay.hpp"
#include "delete_stepper_motor_task.hpp"
#include "inventory.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_data_parser.hpp"
#include "steps_task.hpp"
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
		using DataParser = StepperMotorDataParser<Tstepper_id, Tgpio_id, TaskType>;
		using GpoCreator = typename mcu_server::Creator<mcu_platform::Gpo *(const Tgpio_id&)>;
		using ReportCreator = typename mcu_server::Creator<mcu_server::Data *(int)>;
		
		StepperMotorTasksFactory(
			StepperMotorInventory *inventory,
			const DataParser& stepper_motor_data_parser,
			const GpoCreator& gpo_creator,
			const mcu_platform::Delay& delay,
			const ReportCreator& report_creator
		);
		StepperMotorTasksFactory(const StepperMotorTasksFactory& other);
		StepperMotorTasksFactory& operator=(const StepperMotorTasksFactory& other) = delete;

		mcu_server::Task<mcu_server::Data *(void)> *create(const mcu_server::Data& data) const override;
		mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)> *clone() const override;
	private:
		StepperMotorInventory *m_inventory;
		std::unique_ptr<DataParser> m_stepper_motor_data_parser;
		std::unique_ptr<GpoCreator> m_gpo_creator;
		std::unique_ptr<mcu_platform::Delay> m_delay;
		std::unique_ptr<ReportCreator> m_report_creator;
		
		using TaskCtor = mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)>;
		const std::map<TaskType, std::unique_ptr<TaskCtor>> m_ctors;
	};

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepperMotorTasksFactory(
		StepperMotorInventory *inventory,
		const DataParser& stepper_motor_data_parser,
		const GpoCreator& gpo_creator,
		const mcu_platform::Delay& delay,
		const ReportCreator& report_creator
	):
		m_inventory(inventory),
		m_stepper_motor_data_parser(stepper_motor_data_parser.clone()),
		m_gpo_creator(gpo_creator.clone()),
		m_delay(delay.clone()),
		m_report_creator(report_creator.clone()) {
		
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::StepperMotorTasksFactory(const StepperMotorTasksFactory& other):
		m_inventory(other.m_inventory),
		m_stepper_motor_data_parser(other.stepper_motor_data_parser->clone()),
		m_gpo_creator(other.m_gpo_creator->clone()),
		m_delay(other.m_delay->clone()),
		m_report_creator(other.m_report_creator->clone()) {
		
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline mcu_server::Task<mcu_server::Data *(void)> *StepperMotorTasksFactory<Tstepper_id, Tgpio_id>::create(const mcu_server::Data& data) const {
		auto task_type = m_stepper_motor_data_parser->parse_task_type(data);
		switch (task_type) {
		case TaskType::CREATE_STEPPER_MOTOR:
			return new CreateStepperMotorTask<Tstepper_id, Tgpio_id>(
				m_inventory,
				m_stepper_motor_data_parser->parse_stepper_id(data),
				m_stepper_motor_data_parser->parse_shoulders(data),
				m_stepper_motor_data_parser->parse_states(data),
				*m_gpo_creator,
				*m_delay,
				*m_report_creator
			);
		case TaskType::DELETE_STEPPER_MOTOR:
			return new DeleteStepperMotorTask<Tstepper_id, Tgpio_id>(
				m_inventory,
				m_stepper_motor_data_parser->parse_stepper_id(data),
				*m_report_creator
			);
		case TaskType::STEPS:
			return new StepsTask<Tstepper_id, Tgpio_id>(
				m_inventory,
				m_stepper_motor_data_parser->parse_stepper_id(data),
				m_stepper_motor_data_parser->parse_steps_direction(data),
				m_stepper_motor_data_parser->parse_steps_number(data),
				m_stepper_motor_data_parser->parse_step_duration(data),
				*m_report_creator
			);
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