#include <memory>

#include "gtest/gtest.h"

#include "gpio_tasks_factory.hpp"

using namespace server;
using namespace server_utl;
using namespace mcu_factory;

TEST(ut_gpio_tasks_factory, ctor_dtor_sanity) {
	// WHEN
	GpioTasksFactory<int> *instance_ptr(nullptr);
	GpioTasksFactory<int> *instance_ptr_copy(nullptr);

	// THEN
	// ASSERT_NO_THROW(
	// 	(
	// 		instance_ptr = new GpioTasksFactory<int>(
	// 			inventory(),
	// 			data_parser(),
	// 			CustomCreator<mcu_platform::Gpo *(const GpioId&)>(
	// 				[](const GpioId& gpio_id) {
	// 					return new mcu_platform_uts::TestGpo();
	// 				}
	// 			),
	// 			mcu_platform_uts::TestDelay(),
	// 			CustomCreator<Data *(int)>(
	// 				[](int result) {
	// 					Object report;
	// 					report.add("result", Integer(result));
	// 					return report.clone();
	// 				} 
	// 			)
	// 		)
	// 	)
	// );
	// ASSERT_NE(nullptr, instance_ptr);
	
	// ASSERT_NO_THROW(
	// 	(
	// 		instance_ptr_copy = new GpioTasksFactory<int>(*instance_ptr)
	// 	)
	// );
	// ASSERT_NE(nullptr, instance_ptr_copy);

	// ASSERT_NO_THROW(delete instance_ptr);
	// ASSERT_NO_THROW(delete instance_ptr_copy);
	
	// instance_ptr = nullptr;
	// instance_ptr_copy = nullptr;
}