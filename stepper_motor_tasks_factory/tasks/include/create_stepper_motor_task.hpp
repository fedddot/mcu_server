#ifndef	CREATE_STEPPER_MOTOR_TASK_HPP
#define	CREATE_STEPPER_MOTOR_TASK_HPP

#include "data.hpp"
#include "gpio.hpp"
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
		
		CreateStepperMotorTask(StepperMotorInventory *inventory, const Tstepper_id& id, const Shoulders& shoulders, const States& states);
		CreateStepperMotorTask(const CreateStepperMotorTask& other) = delete;
		CreateStepperMotorTask& operator=(const CreateStepperMotorTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		StepperMotorInventory * const m_inventory;
		const Tstepper_id m_id;
		const Shoulders m_sh;
		const std::unique_ptr<GpioCreator> m_gpio_ctor;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Tstepper_id>
	inline CreateStepperMotorTask<Tstepper_id>::CreateStepperMotorTask(GpioInventory *inventory, const Tstepper_id& id, const mcu_platform::Gpio::Direction& dir, const GpioCreator& gpio_ctor, const ReportCreator& report_ctor): m_inventory(inventory), m_id(id), m_dir(dir), m_gpio_ctor(gpio_ctor.clone()), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Tstepper_id>
	inline mcu_server::Data *CreateStepperMotorTask<Tstepper_id>::execute() const {
		auto gpio_ptr = m_gpio_ctor->create(m_id, m_dir);
		try {
			m_inventory->put(m_id, gpio_ptr);
		} catch (...) {
			delete gpio_ptr;
			gpio_ptr = nullptr;
			throw;
		}
		return m_report_ctor->create(0);
	}
}
#endif // CREATE_STEPPER_MOTOR_TASK_HPP