#include "gtest/gtest.h"
#include "factory_fixture.hpp"
#include "factory.hpp"
#include "gpio_tasks_factory.hpp"
#include "inventory.hpp"

using namespace server;
using namespace mcu_factory;
using namespace mcu_factory_uts;

TEST_F(FactoryFixture, ctor_dtor_sanity) {
	// WHEN
	Factory *instance_ptr(nullptr);
	Factory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new Factory());
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new Factory(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}

using namespace mcu_platform;

TEST_F(FactoryFixture, create_sanity) {
	// GIVEN
	Inventory<class Tkey, class Titem>
	// WHEN
	Factory instance;
	instance.add_task_creator(
		GpioTasksFactory<int>(
			
		)
	)
	// THEN
	ASSERT_NO_THROW(instance_ptr = new Factory());
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new Factory(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}