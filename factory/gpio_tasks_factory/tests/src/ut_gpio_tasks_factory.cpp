#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "default_gpio_tasks_data_retriever.hpp"
#include "gpio_tasks_factory.hpp"
#include "test_platform.hpp"

using namespace server;
using namespace server_utl;
using namespace mcu_factory;

TEST(ut_gpio_tasks_factory, ctor_dtor_sanity) {
	// WHEN
	GpioTasksFactory<int> *instance_ptr(nullptr);
	GpioTasksFactory<int> *instance_ptr_copy(nullptr);
	using GpioInventory = typename GpioTasksFactory<int>::GpioInventory;
	using GpioState = typename GpioTasksFactory<int>::GpioState;
	using Retriever = typename GpioTasksFactory<int>::GpioInventory;

	GpioInventory inventory;
	mcu_platform_uts::TestPlatform<int> platform;
	DefaultGpioTasksDataRetriever<GpioTasksFactory<int>::TaskType, int> retriever;

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new GpioTasksFactory<int>(
				&inventory,
				&platform,
				retriever,
				CustomCreator<Data *(int)>(
					[](int result) {
						Object report;
						report.add("result", Integer(result));
						return report.clone();
					} 
				),
				CustomCreator<Data *(int, const GpioState&)>(
					[](int result, const GpioState& state) {
						Object report;
						report.add("result", Integer(result));
						report.add("state", Integer(static_cast<int>(state)));
						return report.clone();
					}
				)
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new GpioTasksFactory<int>(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}