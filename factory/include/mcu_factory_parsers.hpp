#ifndef	MCU_FACTORY_PARSERS_HPP
#define	MCU_FACTORY_PARSERS_HPP

#include <vector>

#include "array.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "parser.hpp"

namespace mcu_factory {
	template <typename Tgpio_id, typename Ttask_id, typename Ttask_type>
	class McuFactoryParsers {
	public:
		using TaskTypeParser = typename mcu_server::Parser<Ttask_type(const mcu_server::Data&)>;		
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		using GpioIdParser = mcu_server::Parser<Tgpio_id(const mcu_server::Data&)>;
		using GpioDirParser = mcu_server::Parser<GpioDirection(const mcu_server::Data&)>;
		using GpioStateParser = mcu_server::Parser<GpioState(const mcu_server::Data&)>;

		using PersistentTaskIdParser = mcu_server::Parser<Ttask_id(const mcu_server::Data&)>;
		using PersistentTaskDataParser = mcu_server::Parser<mcu_server::Data *(const mcu_server::Data&)>;
		using PersistentTasksIdsParser = mcu_server::Parser<std::vector<Ttask_id>(const mcu_server::Data&)>;

		using DelayParser = mcu_server::Parser<unsigned int(const mcu_server::Data&)>;
		using TasksParser = mcu_server::Parser<mcu_server::Array(const mcu_server::Data&)>;

		virtual ~McuFactoryParsers() noexcept = default;

		virtual const TaskTypeParser& task_type_parser() const = 0;
		virtual const GpioIdParser& gpio_id_parser() const = 0;
		virtual const GpioDirParser& gpio_dir_parser() const = 0;
		virtual const GpioStateParser& gpio_state_parser() const = 0;

		virtual const DelayParser& delay_parser() const = 0;
		virtual const TasksParser& tasks_parser() const = 0;
		
		virtual const PersistentTaskIdParser& persistent_task_id_parser() const = 0;
		virtual const PersistentTaskDataParser& persistent_task_data_parser() const = 0;
		virtual const PersistentTasksIdsParser& persistent_tasks_ids_parser() const = 0;

		virtual McuFactoryParsers *clone() const = 0;
	};
}
#endif // MCU_FACTORY_PARSERS_HPP