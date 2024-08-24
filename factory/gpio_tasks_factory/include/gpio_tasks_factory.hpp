#ifndef	GPIO_TASKS_FACTORY_HPP
#define	GPIO_TASKS_FACTORY_HPP

#include "custom_task.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpio_tasks_data_retriever.hpp"
#include "inventory.hpp"
#include "platform.hpp"
#include "task.hpp"
#include "task_creator.hpp"
#include <memory>
#include <stdexcept>

namespace mcu_factory {
	template <typename Tgpio_id>
	class GpioTasksFactory: public TaskCreator {
	public:
		enum class TaskType: int {
			CREATE_GPIO,
			DELETE_GPIO,
			SET_GPIO,
			GET_GPIO
		};
		
		using FactoryPlatform = mcu_platform::Platform<Tgpio_id>;
		using DataRetriever = GpioTasksDataRetriever<TaskType, Tgpio_id>;
		using ResultReporter = server::Creator<server::Data *(int)>;
		using GpioState = typename mcu_platform::Gpio::State;
		using ResultStateReporter = server::Creator<server::Data *(int, const GpioState&)>;
		using GpioTask = server::Task<server::Data *(void)>;
		using GpioInventory = mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio>;
		
		GpioTasksFactory(
			GpioInventory *inventory,
			const FactoryPlatform *platform,
			const DataRetriever& retriever,
			const ResultReporter& result_reporter,
			const ResultStateReporter& result_state_reporter
		);
		GpioTasksFactory(const GpioTasksFactory& other);
		GpioTasksFactory& operator=(const GpioTasksFactory& other) = delete;

		GpioTask *create(const server::Data& data) const override;
		bool is_creatable(const server::Data& data) const override;
		server::Creator<GpioTask *(const server::Data&)> *clone() const override;
		TaskCreator *clone_task_creator() const override;
	private:
		GpioInventory *m_inventory;
		FactoryPlatform *m_platform;
		const std::unique_ptr<DataRetriever> m_retriever;
		const std::unique_ptr<ResultReporter> m_result_reporter;
		const std::unique_ptr<ResultStateReporter> m_result_state_reporter;
	};

	template <typename Tgpio_id>
	inline GpioTasksFactory<Tgpio_id>::GpioTasksFactory(
		GpioInventory *inventory,
		const FactoryPlatform *platform,
		const DataRetriever& retriever,
		const ResultReporter& result_reporter,
		const ResultStateReporter& result_state_reporter
	):
		m_inventory(inventory),
		m_platform(platform),
		m_retriever(retriever.clone()),
		m_result_reporter(result_reporter.clone()),
		m_result_state_reporter(result_state_reporter.clone()) {
		
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory pointer received");
		}
		if (!m_platform) {
			throw std::invalid_argument("invalid platform pointer received");
		}
	}

	template <typename Tgpio_id>
	inline GpioTasksFactory<Tgpio_id>::GpioTasksFactory(const GpioTasksFactory& other):
		m_inventory(other.m_inventory),
		m_platform(other.m_platform),
		m_retriever(other.m_retriever->clone()),
		m_result_reporter(other.m_result_reporter->clone()),
		m_result_state_reporter(other.m_result_state_reporter->clone()) {
		
	}

	template <typename Tgpio_id>
	inline typename GpioTasksFactory<Tgpio_id>::GpioTask *GpioTasksFactory<Tgpio_id>::create(const server::Data& data) const {
		switch (m_retriever->retrieve_task_type(data)) {
		case TaskType::CREATE_GPIO:
			return new server_utl::CustomTask<server::Data *(void)>(
				[](void)-> server::Data * {
					throw std::invalid_argument("NOT IMPLEMENTED");
				}
			);
		default:
			throw std::invalid_argument("unsupported task type received");
		}
	}
	
	template <typename Tgpio_id>
	inline server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *GpioTasksFactory<Tgpio_id>::clone() const {
		return clone_task_creator();
	}

	template <typename Tgpio_id>
	inline TaskCreator *GpioTasksFactory<Tgpio_id>::clone_task_creator() const {
		return new GpioTasksFactory(*this);
	}
}
#endif // GPIO_TASKS_FACTORY_HPP