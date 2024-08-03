#include "gtest/gtest.h"
#include <string>

#include "data.hpp"
#include "gpio.hpp"
#include "mcu_factory_fixture.hpp"

using namespace mcu_server;
using namespace mcu_factory;
using namespace mcu_factory_uts;
using namespace mcu_platform;

TEST_F(McuFactoryFixture, ctor_dtor_sanity) {
	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(
				platform(),
				parsers(),
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

using CheckReportFunction = std::function<void(const Data&)>;

static void run_create_sanity_tc(const std::string& tc_name, McuFactoryFixture::TestFactory *instance, const Data& test_data, const CheckReportFunction& check_task_report) {
	std::cout << "running TC: " << tc_name << std::endl;
	// WHEN
	std::unique_ptr<Task<Data *(void)>> task(nullptr);
	std::unique_ptr<Data> report(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			task = std::unique_ptr<Task<Data *(void)>>(instance->create(test_data))
		)
	);
	ASSERT_NE(nullptr, task);

	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(task->execute())
		)
	);
	ASSERT_NE(nullptr, report);
	check_task_report(*report);
}

TEST_F(McuFactoryFixture, create_sanity) {
	// GIVEN
	auto check_report = [](const Data& data) {
		auto result = Data::cast<Integer>(Data::cast<Object>(data).access("result")).get();
		ASSERT_EQ(0, result);
	};
	auto check_get_report = [](const Data& data, const Gpio::State& expected_state) {
		auto result = Data::cast<Integer>(Data::cast<Object>(data).access("result")).get();
		ASSERT_EQ(0, result);
		auto state = static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
		ASSERT_EQ(expected_state, state);
	};
	auto check_report_and_task_added = [this](const Data& data, const TaskId& expected_task) {
		auto result = Data::cast<Integer>(Data::cast<Object>(data).access("result")).get();
		ASSERT_EQ(0, result);
		ASSERT_TRUE(platform()->task_inventory()->contains(expected_task));
	};
	auto check_report_and_task_deleted = [this](const Data& data, const TaskId& expected_task) {
		auto result = Data::cast<Integer>(Data::cast<Object>(data).access("result")).get();
		ASSERT_EQ(0, result);
		ASSERT_FALSE(platform()->task_inventory()->contains(expected_task));
	};
	const auto create_task_id = 0;
	const auto delete_task_id = 1;
	using TestCase = std::pair<std::string, std::pair<Object, CheckReportFunction>>;
	const std::vector<TestCase> test_cases{
		{
			"gpi creation",
			{create_gpio_data(1, Gpio::Direction::IN), check_report}
		},
		{
			"persistent task creation (" + std::to_string(create_task_id) + ")",
			{
				create_persistent_task_data(create_task_id, create_gpio_data(100, Gpio::Direction::IN)),
				[check_report_and_task_added, create_task_id](const Data& data) {
					check_report_and_task_added(data, create_task_id);
				}
			}
		},
		{
			"persistent task creation (" + std::to_string(delete_task_id) + ")",
			{
				create_persistent_task_data(delete_task_id, delete_gpio_data(100)),
				[check_report_and_task_added, delete_task_id](const Data& data) {
					check_report_and_task_added(data, delete_task_id);
				}
			}
		},
		{
			"persistent task execution (" + std::to_string(create_task_id) + ")",
			{execute_persistent_task_data(create_task_id), check_report}
		},
		{
			"persistent task execution (" + std::to_string(delete_task_id) + ")",
			{execute_persistent_task_data(delete_task_id), check_report}
		},
		{
			"persistent task deletion (" + std::to_string(create_task_id) + ")",
			{
				delete_persistent_task_data(create_task_id),
				[check_report_and_task_deleted, create_task_id](const Data& data) {
					check_report_and_task_deleted(data, create_task_id);
				}
			}
		},
		{
			"persistent task deletion (" + std::to_string(delete_task_id) + ")",
			{
				delete_persistent_task_data(delete_task_id),
				[check_report_and_task_deleted, delete_task_id](const Data& data) {
					check_report_and_task_deleted(data, delete_task_id);
				}
			}
		},
		{
			"gpo creation",
			{create_gpio_data(2, Gpio::Direction::OUT), check_report}
		},
		{
			"gpo set",
			{set_gpio_data(2, Gpio::State::HIGH), check_report}
		},
		{
			"gpo get",
			{
				get_gpio_data(2),
				[check_get_report](const Data& data) {
					check_get_report(data, Gpio::State::HIGH);
				}
			}
		},
		{
			"gpi get",
			{
				get_gpio_data(1),
				[check_get_report](const Data& data) {
					check_get_report(data, Gpio::State::LOW);
				}
			}
		},
		{
			"gpi deletion",
			{delete_gpio_data(1), check_report}
		},
		{
			"gpo deletion",
			{delete_gpio_data(2), check_report}
		},
		{
			"delay",
			{delay_data(1000), check_report}
		},
		{
			"sequence task",
			{sequence_data(4), check_report}
		}
	};

	// WHEN
	TestFactory instance(
		platform(),
		parsers(),
		result_reporter(),
		result_state_reporter(),
		tasks_results_reporter()
	);

	// THEN
	for (auto test_case: test_cases) {
		run_create_sanity_tc(test_case.first, &instance, test_case.second.first, test_case.second.second);
	}
}