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
		using TaskTypeParser = typename server::Parser<Ttask_type(const server::Data&)>;		
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		using GpioIdParser = server::Parser<Tgpio_id(const server::Data&)>;
		using GpioDirParser = server::Parser<GpioDirection(const server::Data&)>;
		using GpioStateParser = server::Parser<GpioState(const server::Data&)>;

		using PersistentTaskIdParser = server::Parser<Ttask_id(const server::Data&)>;
		using PersistentTaskDataParser = server::Parser<server::Data *(const server::Data&)>;
		using PersistentTasksIdsParser = server::Parser<std::vector<Ttask_id>(const server::Data&)>;

		using DelayParser = server::Parser<unsigned int(const server::Data&)>;
		using TasksParser = server::Parser<server::Array(const server::Data&)>;

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