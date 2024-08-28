#include <memory>

#include "gtest/gtest.h"

#include "data.hpp"
#include "factory_fixture.hpp"
#include "factory.hpp"
#include "task.hpp"

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
	const GpioId test_gpio_id(11);
	const auto test_gpio_dir(Gpio::Direction::OUT);
	const StepperId test_stepper_id(12);
	
	// WHEN
	Factory instance;
	instance.add_task_creator(gpio_tasks_ctor());
	instance.add_task_creator(stepper_motor_tasks_ctor());
	std::unique_ptr<Task<Data *(void)>> task_ptr(nullptr);
	std::unique_ptr<Data> report_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task<Data *(void)>>(instance.create(gpio_data_ctor().create_gpio_data(test_gpio_id, test_gpio_dir))));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);

	ASSERT_NO_THROW(task_ptr = std::unique_ptr<Task<Data *(void)>>(instance.create(gpio_data_ctor().delete_gpio_data(test_gpio_id))));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(task_ptr->execute()));
	ASSERT_NE(nullptr, report_ptr);
}