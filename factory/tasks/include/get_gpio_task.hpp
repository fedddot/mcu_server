#ifndef	GET_GPIO_TASK_HPP
#define	GET_GPIO_TASK_HPP

#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpi.hpp"
#include "gpo.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tgpio_id>
	class GetGpioTask: public server::Task<server::Data *(void)> {
	public:
		using GpioInventory = mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio>;
		using ReportCreator = server::Creator<server::Data *(int, const mcu_platform::Gpio::State&)>;
		
		GetGpioTask(GpioInventory *inventory, const Tgpio_id& id, const ReportCreator& report_ctor);
		GetGpioTask(const GetGpioTask& other) = delete;
		GetGpioTask& operator=(const GetGpioTask& other) = delete;
		
		server::Data *execute() const override;
	private:
		GpioInventory * const m_inventory;
		const Tgpio_id m_id;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Tgpio_id>
	inline GetGpioTask<Tgpio_id>::GetGpioTask(GpioInventory *inventory, const Tgpio_id& id, const ReportCreator& report_ctor): m_inventory(inventory), m_id(id), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Tgpio_id>
	inline server::Data *GetGpioTask<Tgpio_id>::execute() const {
		mcu_platform::Gpio::State retrieved_state(mcu_platform::Gpio::State::LOW);
		auto gpio_ptr = m_inventory->access(m_id);
		switch (gpio_ptr->direction()) {
		case mcu_platform::Gpio::Direction::IN:
			retrieved_state = mcu_platform::Gpio::cast<mcu_platform::Gpi>(*gpio_ptr).state();
			break;
		case mcu_platform::Gpio::Direction::OUT:
			retrieved_state = mcu_platform::Gpio::cast<mcu_platform::Gpo>(*gpio_ptr).state();
			break;
		default:
			throw std::runtime_error("unsupported Gpio direction received");
		}
		return m_report_ctor->create(0, retrieved_state);
	}
}
#endif // GET_GPIO_TASK_HPP