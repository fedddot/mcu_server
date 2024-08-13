#ifndef	CREATE_STEPPER_MOTOR_TASK_HPP
#define	CREATE_STEPPER_MOTOR_TASK_HPP

#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "delay.hpp"
#include "gpo.hpp"
#include "inventory.hpp"
#include "stepper_motor.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id>
	class CreateStepperMotorTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using StepperMotorInventory = mcu_platform::Inventory<Tstepper_id, mcu_platform::StepperMotor<Tgpio_id>>;
		using Shoulders = typename mcu_platform::StepperMotor<Tgpio_id>::Shoulders;
		using States = typename mcu_platform::StepperMotor<Tgpio_id>::States;
		using GpoCreator = typename mcu_server::Creator<mcu_platform::Gpo *(const Tgpio_id&)>;
		using ReportCreator = typename mcu_server::Creator<mcu_server::Data *(int)>;
		
		CreateStepperMotorTask(
			StepperMotorInventory *inventory,
			const Tstepper_id& id,
			const Shoulders& shoulders,
			const States& states,
			const GpoCreator& gpo_ctor,
			const mcu_platform::Delay& delay,
			const ReportCreator& report_ctor
		);
		CreateStepperMotorTask(const CreateStepperMotorTask& other) = delete;
		CreateStepperMotorTask& operator=(const CreateStepperMotorTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		StepperMotorInventory * const m_inventory;
		const Tstepper_id m_id;
		const Shoulders m_shoulders;
		const States m_states;
		const std::unique_ptr<GpoCreator> m_gpo_ctor;
		const std::unique_ptr<ReportCreator> m_report_ctor;
		const std::unique_ptr<mcu_platform::Delay> m_delay;
	};

	template <typename Tstepper_id, typename Tgpio_id>
	inline CreateStepperMotorTask<Tstepper_id, Tgpio_id>::CreateStepperMotorTask(
		StepperMotorInventory *inventory,
		const Tstepper_id& id,
		const Shoulders& shoulders,
		const States& states,
		const GpoCreator& gpo_ctor,
		const mcu_platform::Delay& delay,
		const ReportCreator& report_ctor
	): m_inventory(inventory), m_id(id), m_shoulders(shoulders), m_states(states), m_gpo_ctor(gpo_ctor.clone()), m_delay(delay.clone()), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline mcu_server::Data *CreateStepperMotorTask<Tstepper_id, Tgpio_id>::execute() const {
		auto motor = new mcu_platform::StepperMotor<Tgpio_id>(
			m_shoulders,
			m_states,
			*m_gpo_ctor,
			*m_delay
		);
		try {
			m_inventory->put(m_id, motor);
		} catch (...) {
			delete motor;
			throw;
		}
		return m_report_ctor->create(0);
	}
}
#endif // CREATE_STEPPER_MOTOR_TASK_HPP