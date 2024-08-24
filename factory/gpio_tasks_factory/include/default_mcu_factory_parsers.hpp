#ifndef	DEFAULT_MCU_FACTORY_PARSERS_HPP
#define	DEFAULT_MCU_FACTORY_PARSERS_HPP

#include "custom_parser.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_factory_parsers.hpp"
#include "object.hpp"
#include <vector>

namespace mcu_factory {
	template <typename Tgpio_id, typename Ttask_id, typename Ttask_type>
	class DefaultMcuFactoryParsers: public McuFactoryParsers<Tgpio_id, Ttask_id, Ttask_type> {
	public:
		using Base = McuFactoryParsers<Tgpio_id, Ttask_id, Ttask_type>;
		using TaskTypeParser = typename Base::TaskTypeParser;
		using GpioIdParser = typename Base::GpioIdParser;
		using GpioDirParser = typename Base::GpioDirParser;
		using GpioStateParser = typename Base::GpioStateParser;
		using DelayParser = typename Base::DelayParser;
		using TasksParser = typename Base::TasksParser;
		using PersistentTaskIdParser = typename Base::PersistentTaskIdParser;
		using PersistentTaskDataParser = typename Base::PersistentTaskDataParser;
		using PersistentTasksIdsParser = typename Base::PersistentTasksIdsParser;

		DefaultMcuFactoryParsers();
		DefaultMcuFactoryParsers(const DefaultMcuFactoryParsers& other);
		DefaultMcuFactoryParsers& operator=(const DefaultMcuFactoryParsers& other) = delete;

		const TaskTypeParser& task_type_parser() const override {
			return *m_task_type_parser;
		}
		const GpioIdParser& gpio_id_parser() const override {
			return *m_gpio_id_parser;
		}
		const GpioDirParser& gpio_dir_parser() const override {
			return *m_gpio_dir_parser;
		}
		const GpioStateParser& gpio_state_parser() const override {
			return *m_gpio_state_parser;
		}
		const DelayParser& delay_parser() const override {
			return *m_delay_parser;
		}
		const TasksParser& tasks_parser() const override {
			return *m_tasks_parser;
		}
		
		const PersistentTaskIdParser& persistent_task_id_parser() const override {
			return *m_persistent_task_id_parser;
		}
		const PersistentTaskDataParser& persistent_task_data_parser() const override {
			return *m_persistent_task_data_parser;
		}
		const PersistentTasksIdsParser& persistent_tasks_ids_parser() const override {
			return *m_persistent_tasks_ids_parser;
		}
		Base *clone() const override {
			return new DefaultMcuFactoryParsers(*this);
		}
	private:
		std::unique_ptr<TaskTypeParser> m_task_type_parser;
		std::unique_ptr<GpioIdParser> m_gpio_id_parser;
		std::unique_ptr<GpioDirParser> m_gpio_dir_parser;
		std::unique_ptr<GpioStateParser> m_gpio_state_parser;
		std::unique_ptr<PersistentTaskIdParser> m_persistent_task_id_parser;
		std::unique_ptr<PersistentTaskDataParser> m_persistent_task_data_parser;
		std::unique_ptr<PersistentTasksIdsParser> m_persistent_tasks_ids_parser;
		std::unique_ptr<TasksParser> m_tasks_parser;
		std::unique_ptr<DelayParser> m_delay_parser;
	};

	template <typename Tgpio_id, typename Ttask_id, typename Ttask_type>
	inline DefaultMcuFactoryParsers<Tgpio_id, Ttask_id, Ttask_type>::DefaultMcuFactoryParsers() {
		using namespace server;
		using namespace mcu_platform;
		using namespace server_utl;
		
		m_task_type_parser = std::unique_ptr<TaskTypeParser>(
			new CustomParser<Ttask_type(const Data&)>(
				[](const Data& data) {
					return static_cast<Ttask_type>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
				}
			)
		);
		m_gpio_id_parser = std::unique_ptr<GpioIdParser>(
			new CustomParser<Tgpio_id(const Data&)>(
				[](const Data& data) {
					return static_cast<Tgpio_id>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get());
				}
			)
		);
		m_gpio_dir_parser = std::unique_ptr<GpioDirParser>(
			new CustomParser<Gpio::Direction(const Data&)>(
				[](const Data& data) {
					return static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_dir")).get());
				}
			)
		);
		m_gpio_state_parser = std::unique_ptr<GpioStateParser>(
			new CustomParser<Gpio::State(const Data&)>(
				[](const Data& data) {
					return static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
				}
			)
		);
		m_persistent_task_id_parser = std::unique_ptr<PersistentTaskIdParser>(
			new CustomParser<Ttask_id(const Data&)>(
				[](const Data& data) {
					return static_cast<Ttask_id>(Data::cast<Integer>(Data::cast<Object>(data).access("task_id")).get());
				}
			)
		);
		m_persistent_task_data_parser = std::unique_ptr<PersistentTaskDataParser>(
			new CustomParser<Data *(const Data&)>(
				[](const Data& data) {
					return Data::cast<Object>(data).access("task_data").clone();
				}
			)
		);
		m_persistent_tasks_ids_parser = std::unique_ptr<PersistentTasksIdsParser>(
			new CustomParser<std::vector<Ttask_id>(const Data&)>(
				[](const Data& data) {
					std::vector<Ttask_id> tasks_ids;
					Data::cast<Array>(Data::cast<Object>(data).access("tasks")).for_each(
						[&tasks_ids](int task_index, const Data& task_data) {
							tasks_ids.push_back(static_cast<Ttask_id>(Data::cast<Integer>(task_data).get()));
						}
					);
					return tasks_ids;
				}
			)
		);
		m_tasks_parser = std::unique_ptr<TasksParser>(
			new CustomParser<Array(const Data&)>(
				[](const Data& data) {
					return Array(Data::cast<Array>(Data::cast<Object>(data).access("tasks")));
				}
			)
		);
		m_delay_parser = std::unique_ptr<DelayParser>(
			new CustomParser<unsigned int(const Data&)>(
				[](const Data& data) {
					return static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("delay_ms")).get());
				}
			)
		);
	}

	template <typename Tgpio_id, typename Ttask_id, typename Ttask_type>
	inline DefaultMcuFactoryParsers<Tgpio_id, Ttask_id, Ttask_type>::DefaultMcuFactoryParsers(const DefaultMcuFactoryParsers& other):
		m_task_type_parser(other.m_task_type_parser->clone()),
		m_gpio_id_parser(other.m_gpio_id_parser->clone()),
		m_gpio_dir_parser(other.m_gpio_dir_parser->clone()),
		m_gpio_state_parser(other.m_gpio_state_parser->clone()),
		m_persistent_task_id_parser(other.m_persistent_task_id_parser->clone()),
		m_persistent_tasks_ids_parser(other.m_persistent_tasks_ids_parser->clone()),
		m_persistent_task_data_parser(other.m_persistent_task_data_parser->clone()),
		m_tasks_parser(other.m_tasks_parser->clone()),
		m_delay_parser(other.m_delay_parser->clone()) {

	}
}
#endif // DEFAULT_MCU_FACTORY_PARSERS_HPP