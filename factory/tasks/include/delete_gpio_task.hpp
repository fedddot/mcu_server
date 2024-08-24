#ifndef	DELETE_GPIO_TASK_HPP
#define	DELETE_GPIO_TASK_HPP

#include <memory>

#include "creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tgpio_id>
	class DeleteGpioTask: public server::Task<server::Data *(void)> {
	public:
		using GpioInventory = mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio>;
		using ReportCreator = server::Creator<server::Data *(int)>;
		
		DeleteGpioTask(GpioInventory *inventory, const Tgpio_id& id, const ReportCreator& report_ctor);
		DeleteGpioTask(const DeleteGpioTask& other) = delete;
		DeleteGpioTask& operator=(const DeleteGpioTask& other) = delete;
		
		server::Data *execute() const override;
	private:
		GpioInventory * const m_inventory;
		const Tgpio_id m_id;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Tgpio_id>
	inline DeleteGpioTask<Tgpio_id>::DeleteGpioTask(GpioInventory *inventory, const Tgpio_id& id, const ReportCreator& report_ctor): m_inventory(inventory), m_id(id), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Tgpio_id>
	inline server::Data *DeleteGpioTask<Tgpio_id>::execute() const {
		auto gpio_ptr = m_inventory->pull(m_id);
		delete gpio_ptr;
		gpio_ptr = nullptr;
		return m_report_ctor->create(0);
	}
}
#endif // DELETE_GPIO_TASK_HPP