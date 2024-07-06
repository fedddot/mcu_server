#ifndef	MCU_TASK_ENGINE_HPP
#define	MCU_TASK_ENGINE_HPP

#include "data.hpp"
#include "engine.hpp"
#include "gpio.hpp"
#include "inventory.hpp"
#include "mcu_engine_types.hpp"
#include "mcu_factory.hpp"
#include "mcu_task_type.hpp"
#include "task_engine.hpp"

namespace mcu_task_engine {
	template <typename Tgpio_id>
	class McuTaskEngine: public engine::Engine<engine::Data *(const engine::Data&)> {
	public:
		McuTaskEngine(
			const CtorIdRetriever<McuTaskType>& ctor_id_retriever,
			const FailureReporter& failure_report_creator,
			const GpioIdRetriever<Tgpio_id>& gpio_id_retriever,
			const GpioDirRetriever& gpio_dir_retriever,
			const GpioStateRetriever& gpio_state_retriever,
			const GpioCreator<Tgpio_id>& gpio_creator,
			const GpioReportCreator& gpio_report_creator,
			const GpioGetReportCreator& gpio_get_report_creator);
		McuTaskEngine(const McuTaskEngine& other) = delete;
		McuTaskEngine& operator=(const McuTaskEngine& other) = delete;

		engine::Data *run(const engine::Data& cfg) const override;
	private:
		using GpioInventory = Inventory<Tgpio_id, Gpio>;
		GpioInventory m_gpio_inventory;
		engine::TaskEngine m_engine;
	};

	template <typename Tgpio_id>
	McuTaskEngine<Tgpio_id>::McuTaskEngine(
		const CtorIdRetriever<McuTaskType>& ctor_id_retriever,
		const FailureReporter& failure_report_creator,
		const GpioIdRetriever<Tgpio_id>& gpio_id_retriever,
		const GpioDirRetriever& gpio_dir_retriever,
		const GpioStateRetriever& gpio_state_retriever,
		const GpioCreator<Tgpio_id>& gpio_creator,
		const GpioReportCreator& gpio_report_creator,
		const GpioGetReportCreator& gpio_get_report_creator
	): m_engine(
		McuFactory<Tgpio_id>(
			ctor_id_retriever,
			&m_gpio_inventory,
			gpio_id_retriever,
			gpio_dir_retriever,
			gpio_state_retriever,
			gpio_creator,
			gpio_report_creator,
			gpio_get_report_creator
		), failure_report_creator) {

	}

	template <typename Tgpio_id>
	engine::Data *McuTaskEngine<Tgpio_id>::run(const engine::Data& cfg) const {
		return m_engine.run(cfg);
	}
}

#endif // MCU_TASK_ENGINE_HPP