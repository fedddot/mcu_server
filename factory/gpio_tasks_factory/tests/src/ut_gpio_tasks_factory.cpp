#include "gtest/gtest.h"

#include "gpio_tasks_factory.hpp"
#include "gpio_tasks_factory_fixture.hpp"

using namespace server;
using namespace server_utl;
using namespace mcu_factory;
using namespace mcu_factory_uts;

TEST_F(GpioTasksFactoryFixture, ctor_dtor_sanity) {
	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(
				inventory(),
				platform(),
				retriever(),
				result_reporter(),
				result_state_reporter()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(instance_ptr_copy = new GpioTasksFactory<int>(*instance_ptr));
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}