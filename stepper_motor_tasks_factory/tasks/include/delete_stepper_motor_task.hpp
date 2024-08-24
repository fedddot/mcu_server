#ifndef	DELETE_GPIO_TASK_HPP
#define	DELETE_GPIO_TASK_HPP

#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "stepper_motor.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tstepper_id, typename Tgpio_id>
	class DeleteStepperMotorTask: public server::Task<server::Data *(void)> {
	public:
		using StepperMotorInventory = mcu_platform::Inventory<Tstepper_id, mcu_platform::StepperMotor<Tgpio_id>>;
		using ReportCreator = typename server::Creator<server::Data *(int)>;
		
		DeleteStepperMotorTask(
			StepperMotorInventory *inventory,
			const Tstepper_id& id,
			const ReportCreator& report_ctor
		);
		DeleteStepperMotorTask(const DeleteStepperMotorTask& other) = delete;
		DeleteStepperMotorTask& operator=(const DeleteStepperMotorTask& other) = delete;
		
		server::Data *execute() const override;
	private:
		StepperMotorInventory * const m_inventory;
		const Tstepper_id m_id;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <typename Tstepper_id, typename Tgpio_id>
	inline DeleteStepperMotorTask<Tstepper_id, Tgpio_id>::DeleteStepperMotorTask(
		StepperMotorInventory *inventory,
		const Tstepper_id& id,
		const ReportCreator& report_ctor
	): m_inventory(inventory), m_id(id), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <typename Tstepper_id, typename Tgpio_id>
	inline server::Data *DeleteStepperMotorTask<Tstepper_id, Tgpio_id>::execute() const {
		delete m_inventory->pull(m_id);
		return m_report_ctor->create(0);
	}
}
#endif // DELETE_GPIO_TASK_HPP