#ifndef	GPIO_TASKS_DATA_RETRIEVER_HPP
#define	GPIO_TASKS_DATA_RETRIEVER_HPP

#include "data.hpp"
#include "gpio.hpp"

namespace mcu_factory {
	template <typename Tdomain, typename Ttask_type, typename Tgpio_id>
	class GpioTasksDataRetriever {
	public:
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		
		virtual ~GpioTasksDataRetriever() noexcept = default;
		virtual Tdomain retrieve_domain(const server::Data& data) const = 0;
		virtual Ttask_type retrieve_task_type(const server::Data& data) const = 0;
		virtual Tgpio_id retrieve_gpio_id(const server::Data& data) const = 0;
		virtual GpioDirection retrieve_gpio_dir(const server::Data& data) const = 0;
		virtual GpioState retrieve_gpio_state(const server::Data& data) const = 0;
		virtual GpioTasksDataRetriever *clone() const = 0;
	};
}
#endif // GPIO_TASKS_DATA_RETRIEVER_HPP