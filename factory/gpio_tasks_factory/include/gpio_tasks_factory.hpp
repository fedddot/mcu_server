#ifndef	GPIO_TASKS_FACTORY_HPP
#define	GPIO_TASKS_FACTORY_HPP

#include <memory>
#include <stdexcept>

#include "custom_task.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpio_tasks_data_retriever.hpp"
#include "inventory.hpp"
#include "platform.hpp"
#include "task.hpp"
#include "task_creator.hpp"

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
		mutable GpioInventory *m_inventory;
		const FactoryPlatform *m_platform;
		const std::unique_ptr<DataRetriever> m_retriever;
		const std::unique_ptr<ResultReporter> m_result_reporter;
		const std::unique_ptr<ResultStateReporter> m_result_state_reporter;

		using GpioDirection = typename mcu_platform::Gpio::Direction;
		GpioTask *create_task(const Tgpio_id& id, const GpioDirection& dir) const;
		GpioTask *delete_task(const Tgpio_id& id) const;
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
			return create_task(m_retriever->retrieve_gpio_id(data), m_retriever->retrieve_gpio_dir(data));
		case TaskType::DELETE_GPIO:
			return delete_task(m_retriever->retrieve_gpio_id(data));
		case TaskType::SET_GPIO:
			throw std::invalid_argument("NOT IMPLEMENTED");
		case TaskType::GET_GPIO:
			throw std::invalid_argument("NOT IMPLEMENTED");
		default:
			throw std::invalid_argument("unsupported task type received");
		}
	}

	template <typename Tgpio_id>
	inline bool GpioTasksFactory<Tgpio_id>::is_creatable(const server::Data& data) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
	
	template <typename Tgpio_id>
	inline server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *GpioTasksFactory<Tgpio_id>::clone() const {
		return clone_task_creator();
	}

	template <typename Tgpio_id>
	inline TaskCreator *GpioTasksFactory<Tgpio_id>::clone_task_creator() const {
		return new GpioTasksFactory(*this);
	}

	template <typename Tgpio_id>
	inline typename GpioTasksFactory<Tgpio_id>::GpioTask *GpioTasksFactory<Tgpio_id>::create_task(const Tgpio_id& id, const GpioDirection& dir) const {
		using namespace server;
		using namespace server_utl;
		using namespace mcu_platform;
		auto inventory(m_inventory);
		auto platform(m_platform);
		std::shared_ptr<ResultReporter> reporter(m_result_reporter->clone());
		return new CustomTask<Data *(void)>(
			[id, dir, inventory, platform, reporter](void)-> Data * {
				Gpio *gpio_ptr(nullptr);
				try {
					gpio_ptr = platform->create_gpio(id, dir);
					inventory->put(id, gpio_ptr);
					return reporter->create(0);
				} catch (...) {
					delete gpio_ptr;
					throw;
				}
			}
		);
	}

	template <typename Tgpio_id>
	inline typename GpioTasksFactory<Tgpio_id>::GpioTask *GpioTasksFactory<Tgpio_id>::delete_task(const Tgpio_id& id) const {
		using namespace server;
		using namespace server_utl;
		using namespace mcu_platform;
		auto inventory(m_inventory);
		std::shared_ptr<ResultReporter> reporter(m_result_reporter->clone());
		return new CustomTask<Data *(void)>(
			[id, inventory, reporter](void)-> Data * {
				delete inventory->pull(id);
				return reporter->create(0);
			}
		);
	}
}
#endif // GPIO_TASKS_FACTORY_HPP