#include "gtest/gtest.h"

#include "create_persistent_task.hpp"
#include "gpio.hpp"
#include "tasks_fixture.hpp"

using namespace mcu_server;
using namespace mcu_factory;
using namespace mcu_factory_uts;
using namespace mcu_platform;

TEST_F(TasksFixture, persistent_task_sanity) {
	// GIVEN
	const TaskId test_task_id(10);
	const GpioId test_gpio_id(12);
	const Gpio::Direction test_gpio_dir(Gpio::Direction::OUT);

	// WHEN
	CreatePersistentTask<TaskId> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new CreatePersistentTask<TaskId>(
				platform()->task_inventory(),
				test_task_id,
				factory(),
				create_gpio_data(test_gpio_id, test_gpio_dir),
				result_reporter()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(instance_ptr->execute());
	ASSERT_TRUE(platform()->task_inventory()->contains(test_task_id));
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;
}