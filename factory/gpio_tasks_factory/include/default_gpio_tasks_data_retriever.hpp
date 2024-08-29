#ifndef	DEFAULT_GPIO_TASKS_DATA_RETRIEVER_HPP
#define	DEFAULT_GPIO_TASKS_DATA_RETRIEVER_HPP

#include "data.hpp"
#include "gpio_tasks_data_retriever.hpp"
#include "gpio_tasks_factory.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "string.hpp"

namespace mcu_factory {
	template <typename Tgpio_id>
	class DefaultGpioTasksDataRetriever: public GpioTasksDataRetriever<typename GpioTasksFactory<Tgpio_id>::TaskType, Tgpio_id> {
	public:
		using TaskType = typename GpioTasksFactory<Tgpio_id>::TaskType;
		using GpioDirection = typename GpioTasksDataRetriever<TaskType, Tgpio_id>::GpioDirection;
		using GpioState = typename GpioTasksDataRetriever<TaskType, Tgpio_id>::GpioState;

		DefaultGpioTasksDataRetriever() = default;
		DefaultGpioTasksDataRetriever(const DefaultGpioTasksDataRetriever& other) = default;
		DefaultGpioTasksDataRetriever& operator=(const DefaultGpioTasksDataRetriever& other) = delete;
		
		bool is_gpio_task_creatable(const server::Data& data) const override {
			const std::string domain_name("gpio_tasks");
			using namespace server;
			return domain_name == Data::cast<String>(Data::cast<Object>(data).access("domain")).get();
		}
		TaskType retrieve_task_type(const server::Data& data) const override {
			using namespace server;
			return static_cast<TaskType>(Data::cast<Integer>(Data::cast<Object>(data).access("task_type")).get());
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
		GpioTasksDataRetriever<TaskType, Tgpio_id> *clone() const override {
			return new DefaultGpioTasksDataRetriever(*this);
		}
	};
}
#endif // DEFAULT_GPIO_TASKS_DATA_RETRIEVER_HPP