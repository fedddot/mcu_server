#ifndef	SET_GPIO_TASK_HPP
#define	SET_GPIO_TASK_HPP

#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tgpio_id>
	class SetGpioTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using GpioInventory = mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio>;
		using ReportCreator = mcu_server::Creator<mcu_server::Data *(int)>;
		
		SetGpioTask(GpioInventory *inventory, const Tgpio_id& id, const mcu_platform::Gpio::State& state, const ReportCreator& report_ctor);
		SetGpioTask(const SetGpioTask& other) = delete;
		SetGpioTask& operator=(const SetGpioTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		GpioInventory * const m_inventory;
		const Tgpio_id m_id;
		const mcu_platform::Gpio::State m_state;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Tgpio_id>
	inline SetGpioTask<Tgpio_id>::SetGpioTask(GpioInventory *inventory, const Tgpio_id& id, const mcu_platform::Gpio::State& state, const ReportCreator& report_ctor): m_inventory(inventory), m_id(id), m_state(state), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Tgpio_id>
	inline mcu_server::Data *SetGpioTask<Tgpio_id>::execute() const {
		auto gpio_ptr = m_inventory->access(m_id);
		if (mcu_platform::Gpio::Direction::OUT != gpio_ptr->direction()) {
			throw std::runtime_error("gpio with specified ID is not an output");
		}
		mcu_platform::Gpio::cast<mcu_platform::Gpo>(*gpio_ptr).set_state(m_state);
		return m_report_ctor->create(0);
	}
}
#endif // SET_GPIO_TASK_HPP