#ifndef	MCU_SERVER_HPP
#define	MCU_SERVER_HPP

#include "mcu_engine_types.hpp"
#include "mcu_task_type.hpp"

namespace mcu_server {
	template <typename Tgpio_id>
	class McuServer {
	public:
		using CtorIdRetriever = mcu_task_engine::CtorIdRetriever<mcu_task_engine::McuTaskType>;
		// const FailureReporter& failure_report_creator,
		// const GpioIdRetriever<Tgpio_id>& gpio_id_retriever,
		// const GpioDirRetriever& gpio_dir_retriever,
		// const GpioStateRetriever& gpio_state_retriever,
		// const GpioCreator<Tgpio_id>& gpio_creator,
		// const GpioReportCreator& gpio_report_creator,
		// const GpioGetReportCreator& gpio_get_report_creator);
	// 	McuServer(
	// 		const CtorIdRetriever<McuTaskType>& ctor_id_retriever,
	// 		const FailureReporter& failure_report_creator,
	// 		const GpioIdRetriever<Tgpio_id>& gpio_id_retriever,
	// 		const GpioDirRetriever& gpio_dir_retriever,
	// 		const GpioStateRetriever& gpio_state_retriever,
	// 		const GpioCreator<Tgpio_id>& gpio_creator,
	// 		const GpioReportCreator& gpio_report_creator,
	// 		const GpioGetReportCreator& gpio_get_report_creator);
	// 	McuServer(const McuServer& other) = delete;
	// 	McuServer& operator=(const McuServer& other) = delete;

	// 	engine::Data *run(const engine::Data& cfg) const override;
	// private:
	// 	using GpioInventory = Inventory<Tgpio_id, Gpio>;
	// 	GpioInventory m_gpio_inventory;
	// 	engine::TaskEngine m_engine;
	};

	// template <typename Tgpio_id>
	// McuServer<Tgpio_id>::McuServer(
	// 	const CtorIdRetriever<McuTaskType>& ctor_id_retriever,
	// 	const FailureReporter& failure_report_creator,
	// 	const GpioIdRetriever<Tgpio_id>& gpio_id_retriever,
	// 	const GpioDirRetriever& gpio_dir_retriever,
	// 	const GpioStateRetriever& gpio_state_retriever,
	// 	const GpioCreator<Tgpio_id>& gpio_creator,
	// 	const GpioReportCreator& gpio_report_creator,
	// 	const GpioGetReportCreator& gpio_get_report_creator
	// ): m_engine(
	// 	McuFactory<Tgpio_id>(
	// 		ctor_id_retriever,
	// 		&m_gpio_inventory,
	// 		gpio_id_retriever,
	// 		gpio_dir_retriever,
	// 		gpio_state_retriever,
	// 		gpio_creator,
	// 		gpio_report_creator,
	// 		gpio_get_report_creator
	// 	), failure_report_creator) {

	// }

	// template <typename Tgpio_id>
	// engine::Data *McuServer<Tgpio_id>::run(const engine::Data& cfg) const {
	// 	return m_engine.run(cfg);
	// }
}

#endif // MCU_SERVER_HPP