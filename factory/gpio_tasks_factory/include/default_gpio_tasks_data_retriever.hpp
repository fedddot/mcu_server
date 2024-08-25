#ifndef	DEFAULT_GPIO_TASKS_DATA_RETRIEVER_HPP
#define	DEFAULT_GPIO_TASKS_DATA_RETRIEVER_HPP

#include "data.hpp"
#include "gpio_tasks_data_retriever.hpp"
#include "integer.hpp"
#include "object.hpp"

namespace mcu_factory {
	template <typename Tdomain, typename Ttask_type, typename Tgpio_id>
	class DefaultGpioTasksDataRetriever: public GpioTasksDataRetriever<Tdomain, Ttask_type, Tgpio_id> {
	public:
		using GpioDirection = typename GpioTasksDataRetriever<Tdomain, Ttask_type, Tgpio_id>::GpioDirection;
		using GpioState = typename GpioTasksDataRetriever<Tdomain, Ttask_type, Tgpio_id>::GpioState;

		DefaultGpioTasksDataRetriever() = default;
		DefaultGpioTasksDataRetriever(const DefaultGpioTasksDataRetriever& other) = default;
		DefaultGpioTasksDataRetriever& operator=(const DefaultGpioTasksDataRetriever& other) = delete;
		
		Tdomain retrieve_domain(const server::Data& data) const override {
			using namespace server;
			return static_cast<Tdomain>(Data::cast<Integer>(Data::cast<Object>(data).access("domain")).get());
		}
		Ttask_type retrieve_task_type(const server::Data& data) const override {
			using namespace server;
			return static_cast<Ttask_type>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
		}
		Tgpio_id retrieve_gpio_id(const server::Data& data) const override {
			using namespace server;
			return static_cast<Tgpio_id>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get());
		}
		GpioDirection retrieve_gpio_dir(const server::Data& data) const override {
			using namespace server;
			return static_cast<GpioDirection>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_dir")).get());
		}
		GpioState retrieve_gpio_state(const server::Data& data) const override {
			using namespace server;
			return static_cast<GpioState>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
		}
		GpioTasksDataRetriever<Tdomain, Ttask_type, Tgpio_id> *clone() const override {
			return new DefaultGpioTasksDataRetriever(*this);
		}
	};
}
#endif // DEFAULT_GPIO_TASKS_DATA_RETRIEVER_HPP