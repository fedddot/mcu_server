#ifndef	MCU_FACTORY_HPP
#define	MCU_FACTORY_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "array.hpp"
#include "create_gpio_task.hpp"
#include "create_persistent_task.hpp"
#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "delay.hpp"
#include "delay_task.hpp"
#include "delete_gpio_task.hpp"
#include "delete_persistent_task.hpp"
#include "execute_persistent_task.hpp"
#include "execute_persistent_tasks.hpp"
#include "get_gpio_task.hpp"
#include "gpio.hpp"
#include "mcu_factory_parsers.hpp"
#include "platform.hpp"
#include "sequence_task.hpp"
#include "set_gpio_task.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tgpio_id, typename Ttask_id>
	class McuFactory: public server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> {
	public:
		enum class TaskType: int {
			CREATE_GPIO,
			CREATE_PERSISTENT_TASK,
			DELETE_GPIO,
			DELETE_PERSISTENT_TASK,
			SET_GPIO,
			GET_GPIO,
			EXECUTE_PERSISTENT_TASK,
			EXECUTE_PERSISTENT_TASKS,
			SEQUENCE,
			DELAY
		};
		
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		
		using ResultReporter = server::Creator<server::Data *(int)>;
		using ResultStateReporter = server::Creator<server::Data *(int, const mcu_platform::Gpio::State&)>;
		using TasksResultsReporter = server::Creator<server::Data *(const server::Array&)>;

		using FactoryPlatform = mcu_platform::Platform<Tgpio_id, Ttask_id>;
		using Parsers = McuFactoryParsers<Tgpio_id, Ttask_id, TaskType>;

		McuFactory(
			FactoryPlatform *platform,
			const Parsers& parsers, 
			const ResultReporter& result_reporter,
			const ResultStateReporter& result_state_reporter,
			const TasksResultsReporter& tasks_results_reporter
		);
		McuFactory(const McuFactory& other);
		McuFactory& operator=(const McuFactory& other) = delete;

		server::Task<server::Data *(void)> *create(const server::Data& data) const override;
		server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *clone() const override;
	private:
		FactoryPlatform *m_platform;
		const std::unique_ptr<Parsers> m_parsers;
		const std::unique_ptr<ResultReporter> m_result_reporter;
		const std::unique_ptr<ResultStateReporter> m_result_state_reporter;
		const std::unique_ptr<TasksResultsReporter> m_tasks_results_reporter;

		using GpioCtor = server_utl::CustomCreator<mcu_platform::Gpio *(const Tgpio_id&, const mcu_platform::Gpio::Direction&)>;
		using DelayCtor = server_utl::CustomCreator<mcu_platform::Delay *(void)>;
		
		GpioCtor m_gpio_ctor;
		DelayCtor m_delay_ctor;

		using FactoryTask = server::Task<server::Data *(void)>;
		using TaskCtor = server::Creator<FactoryTask *(const server::Data&)>;
		
		std::map<TaskType, std::unique_ptr<TaskCtor>> m_ctors;

		void init_factory();
	};

	template <typename Tgpio_id, typename Ttask_id>
	inline McuFactory<Tgpio_id, Ttask_id>::McuFactory(
		FactoryPlatform *platform,
		const Parsers& parsers, 
		const ResultReporter& result_reporter,
		const ResultStateReporter& result_state_reporter,
		const TasksResultsReporter& tasks_results_reporter
	):
		m_platform(platform),
		m_parsers(parsers.clone()),
		m_result_reporter(result_reporter.clone()),
		m_result_state_reporter(result_state_reporter.clone()),
		m_tasks_results_reporter(tasks_results_reporter.clone()),
		m_gpio_ctor(
			[this](const Tgpio_id& id, const mcu_platform::Gpio::Direction& dir) {
				return m_platform->create_gpio(id, dir);
			}
		),
		m_delay_ctor(
			[this](void) {
				return m_platform->create_delay();
			}
		) {
		
		if (!m_platform) {
			throw std::invalid_argument("invalid platform pointer received");
		}
		
		init_factory();
	}

	template <typename Tgpio_id, typename Ttask_id>
	inline McuFactory<Tgpio_id, Ttask_id>::McuFactory(const McuFactory& other):
		m_platform(other.m_platform),
		m_parsers(other.m_parsers->clone()),
		m_result_reporter(other.m_result_reporter->clone()),
		m_result_state_reporter(other.m_result_state_reporter->clone()),
		m_tasks_results_reporter(other.m_tasks_results_reporter->clone()),
		m_gpio_ctor(other.m_gpio_ctor),
		m_delay_ctor(other.m_delay_ctor) {
		
		init_factory();
	}

	template <typename Tgpio_id, typename Ttask_id>
	inline server::Task<server::Data *(void)> *McuFactory<Tgpio_id, Ttask_id>::create(const server::Data& data) const {
		auto task_type = (m_parsers->task_type_parser()).parse(data);
		auto task_ctor_iter = m_ctors.find(task_type);
		if (m_ctors.end() == task_ctor_iter) {
			throw std::invalid_argument("task ctor with specified id is not registered");
		}
		return task_ctor_iter->second->create(data);
	}
	
	template <typename Tgpio_id, typename Ttask_id>
	inline server::Creator<server::Task<server::Data *(void)> *(const server::Data&)> *McuFactory<Tgpio_id, Ttask_id>::clone() const {
		return new McuFactory(*this);
	}

	template <typename Tgpio_id, typename Ttask_id>
	inline void McuFactory<Tgpio_id, Ttask_id>::init_factory() {
		using namespace server;
		using namespace server_utl;

		m_ctors.insert(
			{
				TaskType::CREATE_GPIO,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							const Tgpio_id gpio_id((m_parsers->gpio_id_parser()).parse(data));
							const GpioDirection gpio_dir((m_parsers->gpio_dir_parser()).parse(data));
							return new CreateGpioTask<Tgpio_id>(
								m_platform->gpio_inventory(),
								gpio_id,
								gpio_dir,
								m_gpio_ctor,
								*m_result_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::CREATE_PERSISTENT_TASK,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							const Ttask_id task_id((m_parsers->persistent_task_id_parser()).parse(data));
							std::unique_ptr<Data> task_data((m_parsers->persistent_task_data_parser()).parse(data));
							return new CreatePersistentTask<Ttask_id>(
								m_platform->task_inventory(),
								task_id,
								*this,
								*task_data,
								*m_result_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::DELETE_GPIO,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							auto gpio_id = (m_parsers->gpio_id_parser()).parse(data);
							return new DeleteGpioTask<Tgpio_id>(
								m_platform->gpio_inventory(),
								gpio_id,
								*m_result_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::DELETE_PERSISTENT_TASK,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							const Ttask_id task_id((m_parsers->persistent_task_id_parser()).parse(data));
							return new DeletePersistentTask<Ttask_id>(
								m_platform->task_inventory(),
								task_id,
								*m_result_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::SET_GPIO,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							auto gpio_id = (m_parsers->gpio_id_parser()).parse(data);
							auto gpio_state = (m_parsers->gpio_state_parser()).parse(data);
							return new SetGpioTask<Tgpio_id>(
								m_platform->gpio_inventory(),
								gpio_id,
								gpio_state,
								*m_result_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::GET_GPIO,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							auto gpio_id = (m_parsers->gpio_id_parser()).parse(data);
							return new GetGpioTask<Tgpio_id>(
								m_platform->gpio_inventory(),
								gpio_id,
								*m_result_state_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::EXECUTE_PERSISTENT_TASK,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							const Ttask_id task_id((m_parsers->persistent_task_id_parser()).parse(data));
							return new ExecutePersistentTask<Ttask_id>(
								m_platform->task_inventory(),
								task_id
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::EXECUTE_PERSISTENT_TASKS,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							const auto task_ids((m_parsers->persistent_tasks_ids_parser()).parse(data));
							return new ExecutePersistentTasks<Ttask_id>(
								m_platform->task_inventory(),
								task_ids,
								*m_tasks_results_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::SEQUENCE,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							auto tasks = (m_parsers->tasks_parser()).parse(data);
							return new SequenceTask(
								*this,
								tasks,
								*m_tasks_results_reporter
							);
						}
					)
				)
			}
		);
		m_ctors.insert(
			{
				TaskType::DELAY,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							auto delay_ms = (m_parsers->delay_parser()).parse(data);
							return new DelayTask(
								m_delay_ctor,
								delay_ms,
								*m_result_reporter
							);
						}
					)
				)
			}
		);
	}
}
#endif // MCU_FACTORY_HPP