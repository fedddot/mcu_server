#ifndef	STEPS_TASK_HPP
#define	STEPS_TASK_HPP

#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "stepper_motor.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id>
	class StepsTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using StepperMotorInventory = mcu_platform::Inventory<Tstepper_id, mcu_platform::StepperMotor<Tgpio_id>>;
		using Direction = typename mcu_platform::StepperMotor<Tgpio_id>::Direction;
		using ReportCreator = typename mcu_server::Creator<mcu_server::Data *(int)>;
		
		StepsTask(
			StepperMotorInventory *inventory,
			const Tstepper_id& id,
			const Direction& direction,
			const unsigned int steps_number,
			const unsigned int step_duration_ms,
			const ReportCreator& report_ctor
		);
		StepsTask(const StepsTask& other) = delete;
		StepsTask& operator=(const StepsTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		StepperMotorInventory * const m_inventory;
		const Tstepper_id m_id;
		const Direction m_direction;
		const unsigned int m_steps_number;
		const unsigned int m_step_duration_ms;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <typename Tstepper_id, typename Tgpio_id>
	inline StepsTask<Tstepper_id, Tgpio_id>::StepsTask(
		StepperMotorInventory *inventory,
		const Tstepper_id& id,
		const Direction& direction,
		const unsigned int steps_number,
		const unsigned int step_duration_ms,
		const ReportCreator& report_ctor
	): m_inventory(inventory), m_id(id), m_direction(direction), m_steps_number(steps_number), m_step_duration_ms(step_duration_ms), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline mcu_server::Data *StepsTask<Tstepper_id, Tgpio_id>::execute() const {
		m_inventory->access(m_id)->steps(m_direction, m_steps_number, m_step_duration_ms);
		return m_report_ctor->create(0);
	}
}
#endif // STEPS_TASK_HPP