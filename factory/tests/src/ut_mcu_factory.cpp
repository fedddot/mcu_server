#include "gtest/gtest.h"

#include "mcu_factory_fixture.hpp"

using namespace mcu_factory;
using namespace mcu_factory_uts;

TEST_F(McuFactoryFixture, ctor_dtor_sanity) {
	// WHEN
	TestFactory *instance_ptr(nullptr);
	TestFactory *instance_ptr_copy(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestFactory(
				platform(),
				task_type_parser(),
				gpio_id_parser(),
				gpio_dir_parser(),
				gpio_state_parser(),
				tasks_parser(),
				delay_parser(),
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

// using CheckReportFunction = std::function<void(const Data&)>;

// static void run_create_sanity_tc(const std::string& tc_name, McuFactory<GpioId> *instance, const Data& test_data, const CheckReportFunction& check_task_report) {
// 	std::cout << "running TC: " << tc_name << std::endl;
// 	// WHEN
// 	std::unique_ptr<Task<Data *(void)>> task(nullptr);
// 	std::unique_ptr<Data> report(nullptr);

// 	// THEN
// 	ASSERT_NO_THROW(
// 		(
// 			task = std::unique_ptr<Task<Data *(void)>>(instance->create(test_data))
// 		)
// 	);
// 	ASSERT_NE(nullptr, task);

// 	ASSERT_NO_THROW(
// 		(
// 			report = std::unique_ptr<Data>(task->execute())
// 		)
// 	);
// 	ASSERT_NE(nullptr, report);
// 	check_task_report(*report);
// }

// TEST_F(McuFactoryFixture, create_sanity) {
// 	// GIVEN
// 	Inventory<GpioId, Gpio> gpio_inventory;
// 	auto check_report = [](const Data& data) {
// 		auto result = Data::cast<Integer>(Data::cast<Object>(data).access("result")).get();
// 		ASSERT_EQ(0, result);
// 	};
// 	auto check_get_report = [](const Data& data, const Gpio::State& expected_state) {
// 		auto result = Data::cast<Integer>(Data::cast<Object>(data).access("result")).get();
// 		ASSERT_EQ(0, result);
// 		ASSERT_EQ(0, result);
// 		auto state = static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
// 		ASSERT_EQ(expected_state, state);
// 	};
// 	using TestCase = std::pair<std::string, std::pair<Object, CheckReportFunction>>;
// 	const std::vector<TestCase> test_cases{
// 		{
// 			"gpi creation",
// 			{create_gpio_data(1, Gpio::Direction::IN), check_report}
// 		},
// 		{
// 			"gpo creation",
// 			{create_gpio_data(2, Gpio::Direction::OUT), check_report}
// 		},
// 		{
// 			"gpo set",
// 			{set_gpio_data(2, Gpio::State::HIGH), check_report}
// 		},
// 		{
// 			"gpo get",
// 			{
// 				get_gpio_data(2),
// 				[check_get_report](const Data& data) {
// 					check_get_report(data, Gpio::State::HIGH);
// 				}
// 			}
// 		},
// 		{
// 			"gpi get",
// 			{
// 				get_gpio_data(1),
// 				[check_get_report](const Data& data) {
// 					check_get_report(data, Gpio::State::LOW);
// 				}
// 			}
// 		},
// 		{
// 			"gpi deletion",
// 			{delete_gpio_data(1), check_report}
// 		},
// 		{
// 			"delay",
// 			{delay_data(1000), check_report}
// 		},
// 		{
// 			"sequence task",
// 			{sequence_data(4), check_report}
// 		}
// 	};

// 	// WHEN
// 	McuFactory<GpioId> instance(
// 		ctor_id_retriever(),
// 		&gpio_inventory,
// 		gpio_id_retriever(),
// 		gpio_dir_retriever(),
// 		gpio_state_retriever(),
// 		sequence_retriever(),
// 		delay_retriever(),
// 		gpio_creator(),
// 		gpio_report_creator(),
// 		gpio_get_report_creator(),
// 		delay_ctor()
// 	);

// 	// THEN
// 	for (auto test_case: test_cases) {
// 		run_create_sanity_tc(test_case.first, &instance, test_case.second.first, test_case.second.second);
// 	}
// }