#include "gtest/gtest.h"
#include <string>

#include "create_persistent_task.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "tasks_fixture.hpp"

using namespace mcu_server;
using namespace mcu_factory;
using namespace mcu_factory_uts;
using namespace mcu_platform;

TEST_F(TasksFixture, ctor_dtor_sanity) {
	// WHEN
	CreatePersistentTask<int> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new CreatePersistentTask<int>(
				platform()->task_inventory(),
				0,
				result_reporter(),
				result_state_reporter(),
				tasks_results_reporter()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	
	ASSERT_NO_THROW(
		(
			instance_ptr_copy = new TestFactory(*instance_ptr)
		)
	);
	ASSERT_NE(nullptr, instance_ptr_copy);

	ASSERT_NO_THROW(delete instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr_copy);
	
	instance_ptr = nullptr;
	instance_ptr_copy = nullptr;
}