#ifndef	CREATE_GPIO_TASK_HPP
#define	CREATE_GPIO_TASK_HPP

#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tgpio_id>
	class CreateGpioTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using GpioInventory = mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio>;
		using GpioCreator = mcu_server::Creator<mcu_platform::Gpio *(const Tgpio_id&, const mcu_platform::Gpio::Direction&)>;
		using ReportCreator = mcu_server::Creator<mcu_server::Data *(int)>;
		
		CreateGpioTask(GpioInventory *inventory, const Tgpio_id& id, const mcu_platform::Gpio::Direction& dir, const GpioCreator& gpio_ctor, const ReportCreator& report_ctor);
		CreateGpioTask(const CreateGpioTask& other) = delete;
		CreateGpioTask& operator=(const CreateGpioTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		GpioInventory * const m_inventory;
		const Tgpio_id m_id;
		const mcu_platform::Gpio::Direction m_dir;
		const std::unique_ptr<GpioCreator> m_gpio_ctor;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Tgpio_id>
	inline CreateGpioTask<Tgpio_id>::CreateGpioTask(GpioInventory *inventory, const Tgpio_id& id, const mcu_platform::Gpio::Direction& dir, const GpioCreator& gpio_ctor, const ReportCreator& report_ctor): m_inventory(inventory), m_id(id), m_dir(dir), m_gpio_ctor(gpio_ctor.clone()), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Tgpio_id>
	inline mcu_server::Data *CreateGpioTask<Tgpio_id>::execute() const {
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
#endif // CREATE_GPIO_TASK_HPP