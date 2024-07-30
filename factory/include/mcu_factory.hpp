#ifndef	MCU_FACTORY_HPP
#define	MCU_FACTORY_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "array.hpp"
#include "create_gpio_task.hpp"
#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "delay_task.hpp"
#include "delete_gpio_task.hpp"
#include "get_gpio_task.hpp"
#include "gpio.hpp"
#include "parser.hpp"
#include "platform.hpp"
#include "sequence_task.hpp"
#include "set_gpio_task.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Tdata, typename Tgpio_id>
	class McuFactory: public mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)> {
	public:
		enum class TaskType: int {
			CREATE_GPIO,
			DELETE_GPIO,
			SET_GPIO,
			GET_GPIO,
			SEQUENCE,
			DELAY
		};
		using TaskTypeParser = mcu_server::Parser<TaskType(const mcu_server::Data&)>;
		
		using GpioInventory = mcu_platform::Inventory<Tgpio_id, mcu_platform::Gpio>;
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		using GpioIdParser = mcu_server::Parser<Tgpio_id(const mcu_server::Data&)>;
		using GpioDirParser = mcu_server::Parser<GpioDirection(const mcu_server::Data&)>;
		using GpioStateParser = mcu_server::Parser<GpioState(const mcu_server::Data&)>;

		using DelayParser = mcu_server::Parser<unsigned int(const mcu_server::Data&)>;
		using TasksParser = mcu_server::Parser<mcu_server::Array(const mcu_server::Data&)>;

		using ResultReporter = mcu_server::Creator<mcu_server::Data *(int)>;
		using ResultStateReporter = mcu_server::Creator<mcu_server::Data *(int, const mcu_platform::Gpio::State&)>;
		using TasksResultsReporter = mcu_server::Creator<mcu_server::Data *(const mcu_server::Array&)>;

		McuFactory(
			mcu_platform::Platform<Tdata, Tgpio_id> *platform,
			const TaskTypeParser& task_type_parser,
			const GpioIdParser& gpio_id_parser,
			const GpioDirParser& gpio_dir_parser,
			const GpioStateParser& gpio_state_parser,
			const TasksParser& tasks_parser,
			const DelayParser& delay_parser,
			const ResultReporter& result_reporter,
			const ResultStateReporter& result_state_reporter,
			const TasksResultsReporter& tasks_results_reporter
		);
		McuFactory(const McuFactory& other);
		McuFactory& operator=(const McuFactory& other) = delete;

		mcu_server::Task<mcu_server::Data *(void)> *create(const mcu_server::Data& data) const override;
		mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)> *clone() const override;
	private:
		mcu_platform::Platform<Tdata, Tgpio_id> *m_platform;
		const std::unique_ptr<TaskTypeParser> m_task_type_parser;
		const std::unique_ptr<GpioIdParser> m_gpio_id_parser;
		const std::unique_ptr<GpioDirParser> m_gpio_dir_parser;
		const std::unique_ptr<GpioStateParser> m_gpio_state_parser;
		const std::unique_ptr<TasksParser> m_tasks_parser;
		const std::unique_ptr<DelayParser> m_delay_parser;
		const std::unique_ptr<ResultReporter> m_result_reporter;
		const std::unique_ptr<ResultStateReporter> m_result_state_reporter;
		const std::unique_ptr<TasksResultsReporter> m_tasks_results_reporter;

		using FactoryTask = mcu_server::Task<mcu_server::Data *(void)>;
		using TaskCtor = mcu_server::Creator<FactoryTask *(const mcu_server::Data&)>;
		
		std::map<TaskType, std::unique_ptr<TaskCtor>> m_ctors;

		void init_factory();
	};

	template <typename Tdata, typename Tgpio_id>
	inline McuFactory<Tdata, Tgpio_id>::McuFactory(
		mcu_platform::Platform<Tdata, Tgpio_id> *platform,
		const TaskTypeParser& task_type_parser,
		const GpioIdParser& gpio_id_parser,
		const GpioDirParser& gpio_dir_parser,
		const GpioStateParser& gpio_state_parser,
		const TasksParser& tasks_parser,
		const DelayParser& delay_parser,
		const ResultReporter& result_reporter,
		const ResultStateReporter& result_state_reporter,
		const TasksResultsReporter& tasks_results_reporter
	):
		m_platform(platform),
		m_task_type_parser(task_type_parser.clone()),
		m_gpio_id_parser(gpio_id_parser.clone()),
		m_gpio_dir_parser(gpio_dir_parser.clone()),
		m_gpio_state_parser(gpio_state_parser.clone()),
		m_tasks_parser(tasks_parser.clone()),
		m_delay_parser(delay_parser.clone()),
		m_result_reporter(result_reporter.clone()),
		m_result_state_reporter(result_state_reporter.clone()),
		m_tasks_results_reporter(tasks_results_reporter.clone()) {
		
		if (!m_platform) {
			throw std::invalid_argument("invalid platform pointer received");
		}
		
		init_factory();
	}

	template <typename Tdata, typename Tgpio_id>
	inline McuFactory<Tdata, Tgpio_id>::McuFactory(const McuFactory& other):
		m_platform(other.m_platform),
		m_task_type_parser(other.m_task_type_parser->clone()),
		m_gpio_id_parser(other.m_gpio_id_parser->clone()),
		m_gpio_dir_parser(other.m_gpio_dir_parser->clone()),
		m_gpio_state_parser(other.m_gpio_state_parser->clone()),
		m_tasks_parser(other.m_tasks_parser->clone()),
		m_delay_parser(other.m_delay_parser->clone()),
		m_result_reporter(other.m_result_reporter->clone()),
		m_result_state_reporter(other.m_result_state_reporter->clone()),
		m_tasks_results_reporter(other.m_tasks_results_reporter->clone()) {
		
		init_factory();
	}

	template <typename Tdata, typename Tgpio_id>
	inline mcu_server::Task<mcu_server::Data *(void)> *McuFactory<Tdata, Tgpio_id>::create(const mcu_server::Data& data) const {
		auto task_type = m_task_type_parser->parse(data);
		auto task_ctor_iter = m_ctors.find(task_type);
		if (m_ctors.end() == task_ctor_iter) {
			throw std::invalid_argument("task ctor with specified id is not registered");
		}
		return task_ctor_iter->second->create(data);
	}
	
	template <typename Tdata, typename Tgpio_id>
	inline mcu_server::Creator<mcu_server::Task<mcu_server::Data *(void)> *(const mcu_server::Data&)> *McuFactory<Tdata, Tgpio_id>::clone() const {
		return new McuFactory(*this);
	}

	template <typename Tdata, typename Tgpio_id>
	inline void McuFactory<Tdata, Tgpio_id>::init_factory() {
		using namespace mcu_server;
		using namespace mcu_server_utl;
		m_ctors.insert(
			{
				TaskType::CREATE_GPIO,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							const Tgpio_id gpio_id(m_gpio_id_parser->parse(data));
							const GpioDirection gpio_dir(m_gpio_dir_parser->parse(data));
							const CustomCreator<mcu_platform::Gpio *(const Tgpio_id&, const mcu_platform::Gpio::Direction&)> gpio_ctor(
								[this](const Tgpio_id& id, const mcu_platform::Gpio::Direction& dir) {
									return m_platform->create_gpio(id, dir);
								}
							);
							return new CreateGpioTask<Tgpio_id>(
								m_platform->gpio_inventory(),
								gpio_id,
								gpio_dir,
								gpio_ctor,
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
							auto gpio_id = m_gpio_id_parser->parse(data);
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
				TaskType::SET_GPIO,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							auto gpio_id = m_gpio_id_parser->parse(data);
							auto gpio_state = m_gpio_state_parser->parse(data);
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
							auto gpio_id = m_gpio_id_parser->parse(data);
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
				TaskType::SEQUENCE,
				std::unique_ptr<TaskCtor>(
					new CustomCreator<FactoryTask *(const Data&)>(
						[this](const Data& data) {
							auto tasks = m_tasks_parser->parse(data);
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
							auto delay_ms = m_delay_parser->parse(data);
							return new DelayTask(
								[this](unsigned int delay_ms) {
									m_platform->delay(delay_ms);
								},
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