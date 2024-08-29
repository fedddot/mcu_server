#include <iostream>
#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "gpio.hpp"
#include "gpio_test_data_creator.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_fixture.hpp"
#include "stepper_motor_test_data_creator.hpp"

using namespace server;
using namespace server_uts;
using namespace mcu_platform;
using namespace mcu_factory_uts;

TEST_F(ServerFixture, ctor_dtor_sanity) {
	// WHEN
	Server *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new Server(factory(), fail_report_creator()));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

using CheckReportFunction = std::function<void(const Data&)>;
inline static void run_sanity_tc(const std::string& title, Server *instance_ptr, const Data& test_data, const CheckReportFunction& report_checker) {
	// WHEN
	std::unique_ptr<Data> report_ptr(nullptr);

	// THEN
	std::cout << "running TC: " << title << std::endl;
	ASSERT_NO_THROW(report_ptr = std::unique_ptr<Data>(instance_ptr->run(test_data)));
	ASSERT_NE(nullptr, report_ptr);
	report_checker(*report_ptr);
}

TEST_F(ServerFixture, run_sanity) {
	// GIVEN
	auto check_report = [](const Data& data) {
		ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
	};
	auto check_gpio_state_report = [check_report](const Data& data, const Gpio::State& expected_state) {
		check_report(data);
		ASSERT_EQ(expected_state, static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get()));
	};

	const GpioId test_gpi_id(12);
	const GpioId test_gpo_id(13);
	
	using TestCase = std::pair<std::string, std::pair<Object, CheckReportFunction>>;
	using GpioDirection = typename Gpio::Direction;
	using GpioState = typename Gpio::State;

	const std::vector<TestCase> test_cases{
		{
			"gpi creation",
			{
				GpioTestDataCreator().create_gpio_data(test_gpi_id, GpioDirection::IN),
				check_report
			}
		},
		{
			"gpo creation",
			{
				GpioTestDataCreator().create_gpio_data(test_gpo_id, GpioDirection::OUT),
				check_report
			}
		},
		{
			"gpo set",
			{
				GpioTestDataCreator().set_gpio_data(test_gpo_id, GpioState::HIGH),
				check_report
			}
		},
		{
			"gpo get",
			{
				GpioTestDataCreator().get_gpio_data(test_gpo_id),
				[check_gpio_state_report](const Data& data) {
					check_gpio_state_report(data, GpioState::HIGH);
				}
			}
		},
		{
			"gpi get",
			{
				GpioTestDataCreator().get_gpio_data(test_gpi_id),
				[check_gpio_state_report](const Data& data) {
					check_gpio_state_report(data, GpioState::LOW);
				}
			}
		},
		{
			"gpi deletion",
			{
				GpioTestDataCreator().delete_gpio_data(test_gpi_id),
				check_report
			}
		},
		{
			"gpo deletion",
			{
				GpioTestDataCreator().delete_gpio_data(test_gpo_id),
				check_report
			}
		}
	};
	Server instance(factory(), fail_report_creator());

	// THEN
	for (const auto& [title, tc_data]: test_cases) {
		run_sanity_tc(title, &instance, tc_data.first, tc_data.second);
	}
}

// TEST_F(ServerFixture, create_and_run_persistent_tasks_sanity) {
// 	// GIVEN
// 	auto check_report = [this](const McuData& data) {
// 		std::unique_ptr<Data> parsed_data(parser().parse(data));
// 		auto result = Data::cast<Integer>(Data::cast<Object>(*parsed_data).access("result")).get();
// 		if (0 != result) {
// 			std::cout << "failure report received (what: " << Data::cast<String>(Data::cast<Object>(*parsed_data).access("what")).get() << ")" << std::endl;
// 		}
// 		ASSERT_EQ(0, result);
// 	};

// 	auto check_report_and_gpio_state = [check_report, this](const McuData& data, const GpioId& gpio_id, const Gpio::State& expected_state) {
// 		check_report(data);
// 		ASSERT_TRUE(platform()->gpio_inventory()->contains(gpio_id));
// 		const auto& gpio_casted = Gpio::cast<Gpo>(*(platform()->gpio_inventory()->access(gpio_id)));
// 		ASSERT_EQ(expected_state, gpio_casted.state());
// 	};

// 	const GpioId test_gpio_id(14);
	
// 	const TaskId test_set_high_task_id(17);
// 	const auto set_high_data(set_gpio_data(test_gpio_id, Gpio::State::HIGH));

// 	const TaskId test_set_low_task_id(18);
// 	const auto set_low_data(set_gpio_data(test_gpio_id, Gpio::State::LOW));

// 	const TaskId test_delay_task_id(20);
// 	const int test_delay_ms(1000);
// 	const auto test_delay_data(delay_data(test_delay_ms));

// 	Array tasks_ids;
// 	tasks_ids.push_back(Integer(test_set_high_task_id));
// 	tasks_ids.push_back(Integer(test_delay_task_id));
// 	tasks_ids.push_back(Integer(test_set_low_task_id));
	
// 	using TestCase = std::pair<std::string, std::pair<ServerFixture::McuData, CheckReportFunction>>;
	
// 	const std::vector<TestCase> test_cases{
// 		{
// 			"create persistent set gpio task (to HIGH state)",
// 			{
// 				serializer().serialize(create_persistent_task_data(test_set_high_task_id, set_high_data)),
// 				check_report
// 			}
// 		},
// 		{
// 			"create persistent set gpio task (to LOW state)",
// 			{
// 				serializer().serialize(create_persistent_task_data(test_set_low_task_id, set_low_data)),
// 				check_report
// 			}
// 		},
// 		{
// 			"create persistent delay task",
// 			{
// 				serializer().serialize(create_persistent_task_data(test_delay_task_id, test_delay_data)),
// 				check_report
// 			}
// 		},
// 		{
// 			"execute tasks",
// 			{
// 				serializer().serialize(execute_persistent_tasks_data(tasks_ids)),
// 				check_report
// 			}
// 		},
// 		{
// 			"delete persistent set gpio task (to HIGH state)",
// 			{
// 				serializer().serialize(delete_persistent_task_data(test_set_high_task_id)),
// 				check_report
// 			}
// 		},
// 		{
// 			"delete persistent set gpio task (to LOW state)",
// 			{
// 				serializer().serialize(delete_persistent_task_data(test_set_low_task_id)),
// 				check_report
// 			}
// 		},
// 		{
// 			"delete persistent delay task",
// 			{
// 				serializer().serialize(delete_persistent_task_data(test_delay_task_id)),
// 				check_report
// 			}
// 		}
// 	};

// 	// WHEN
// 	platform()->gpio_inventory()->put(test_gpio_id, new mcu_platform_uts::TestGpo());

// 	// THEN
// 	for (auto test_case: test_cases) {
// 		run_create_sanity_tc(test_case.first, test_case.second.first, test_case.second.second, this);
// 	}

// 	// CLEAN UP
// 	delete platform()->gpio_inventory()->pull(test_gpio_id);
// }