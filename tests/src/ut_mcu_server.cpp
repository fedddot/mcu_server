#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "array.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "mcu_server_fixture.hpp"
#include "object.hpp"
#include "string.hpp"
#include "test_gpo.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;
using namespace mcu_server_uts;
using namespace mcu_platform;

TEST_F(McuServerFixture, ctor_dtor_sanity) {
	// WHEN
	TestMcuServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestMcuServer(
				parser(),
				serializer(),
				factory(),
				fail_report_creator()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

using CheckReportFunction = std::function<void(const McuServerFixture::McuData&)>;

static void run_create_sanity_tc(const std::string& tc_name, const McuServerFixture::McuData& test_data, const CheckReportFunction& check_task_report, McuServerFixture *fixture) {
	std::cout << "running TC: " << tc_name << std::endl;
	std::cout << "test data: " << test_data << std::endl;

	// WHEN
	McuServerFixture::McuData report("");
	McuServerFixture::TestMcuServer instance(
		fixture->parser(),
		fixture->serializer(),
		fixture->factory(),
		fixture->fail_report_creator()
	);

	// THEN
	ASSERT_NO_THROW(report = instance.run(test_data));
	check_task_report(report);
}

TEST_F(McuServerFixture, run_sanity) {
	// GIVEN
	auto check_report = [this](const McuServerFixture::McuData& data) {
		std::unique_ptr<Data> parsed_data(parser().parse(data));
		auto result = Data::cast<Integer>(Data::cast<Object>(*parsed_data).access("result")).get();
		ASSERT_EQ(0, result);
	};
	auto check_get_report = [this](const McuServerFixture::McuData& data, const Gpio::State& expected_state) {
		std::unique_ptr<Data> parsed_data(parser().parse(data));
		auto result = Data::cast<Integer>(Data::cast<Object>(*parsed_data).access("result")).get();
		ASSERT_EQ(0, result);
		ASSERT_EQ(0, result);
		auto state = static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(*parsed_data).access("gpio_state")).get());
		ASSERT_EQ(expected_state, state);
	};
	using TestCase = std::pair<std::string, std::pair<McuServerFixture::McuData, CheckReportFunction>>;
	const std::vector<TestCase> test_cases{
		{
			"gpi creation",
			{
				serializer().serialize(create_gpio_data(1, Gpio::Direction::IN)),
				check_report
			}
		},
		{
			"gpo creation",
			{
				serializer().serialize(create_gpio_data(2, Gpio::Direction::OUT)),
				check_report
			}
		},
		{
			"gpo set",
			{
				serializer().serialize(set_gpio_data(2, Gpio::State::HIGH)),
				check_report
			}
		},
		{
			"gpo get",
			{
				serializer().serialize(get_gpio_data(2)),
				[check_get_report](const McuServerFixture::McuData& data) {
					check_get_report(data, Gpio::State::HIGH);
				}
			}
		},
		{
			"gpi get",
			{
				serializer().serialize(get_gpio_data(1)),
				[check_get_report](const McuServerFixture::McuData& data) {
					check_get_report(data, Gpio::State::LOW);
				}
			}
		},
		{
			"gpi deletion",
			{
				serializer().serialize(delete_gpio_data(1)),
				check_report
			}
		},
		{
			"gpo deletion",
			{
				serializer().serialize(delete_gpio_data(2)),
				check_report
			}
		},
		{
			"delay",
			{
				serializer().serialize(delay_data(1000)),
				check_report
			}
		},
		{
			"sequence task",
			{
				serializer().serialize(sequence_data(4)),
				check_report
			}
		}
	};

	// THEN
	for (auto test_case: test_cases) {
		run_create_sanity_tc(test_case.first, test_case.second.first, test_case.second.second, this);
	}
}

TEST_F(McuServerFixture, create_and_run_persistent_tasks_sanity) {
	// GIVEN
	auto check_report = [this](const McuData& data) {
		std::unique_ptr<Data> parsed_data(parser().parse(data));
		auto result = Data::cast<Integer>(Data::cast<Object>(*parsed_data).access("result")).get();
		if (0 != result) {
			std::cout << "failure report received (what: " << Data::cast<String>(Data::cast<Object>(*parsed_data).access("what")).get() << ")" << std::endl;
		}
		ASSERT_EQ(0, result);
	};

	auto check_report_and_gpio_state = [check_report, this](const McuData& data, const GpioId& gpio_id, const Gpio::State& expected_state) {
		check_report(data);
		ASSERT_TRUE(platform()->gpio_inventory()->contains(gpio_id));
		const auto& gpio_casted = Gpio::cast<Gpo>(*(platform()->gpio_inventory()->access(gpio_id)));
		ASSERT_EQ(expected_state, gpio_casted.state());
	};

	const GpioId test_gpio_id(14);
	
	const TaskId test_set_high_task_id(17);
	const auto set_high_data(set_gpio_data(test_gpio_id, Gpio::State::HIGH));

	const TaskId test_set_low_task_id(18);
	const auto set_low_data(set_gpio_data(test_gpio_id, Gpio::State::LOW));

	const TaskId test_delay_task_id(20);
	const int test_delay_ms(1000);
	const auto test_delay_data(delay_data(test_delay_ms));

	Array tasks_ids;
	tasks_ids.push_back(Integer(test_set_high_task_id));
	tasks_ids.push_back(Integer(test_delay_task_id));
	tasks_ids.push_back(Integer(test_set_low_task_id));
	
	using TestCase = std::pair<std::string, std::pair<McuServerFixture::McuData, CheckReportFunction>>;
	
	const std::vector<TestCase> test_cases{
		{
			"create persistent set gpio task (to HIGH state)",
			{
				serializer().serialize(create_persistent_task_data(test_set_high_task_id, set_high_data)),
				check_report
			}
		},
		{
			"create persistent set gpio task (to LOW state)",
			{
				serializer().serialize(create_persistent_task_data(test_set_low_task_id, set_low_data)),
				check_report
			}
		},
		{
			"create persistent delay task",
			{
				serializer().serialize(create_persistent_task_data(test_delay_task_id, test_delay_data)),
				check_report
			}
		},
		{
			"execute tasks",
			{
				serializer().serialize(execute_persistent_tasks_data(tasks_ids)),
				check_report
			}
		},
		{
			"delete persistent set gpio task (to HIGH state)",
			{
				serializer().serialize(delete_persistent_task_data(test_set_high_task_id)),
				check_report
			}
		},
		{
			"delete persistent set gpio task (to LOW state)",
			{
				serializer().serialize(delete_persistent_task_data(test_set_low_task_id)),
				check_report
			}
		},
		{
			"delete persistent delay task",
			{
				serializer().serialize(delete_persistent_task_data(test_delay_task_id)),
				check_report
			}
		}
	};

	// WHEN
	platform()->gpio_inventory()->put(test_gpio_id, new mcu_platform_uts::TestGpo());

	// THEN
	for (auto test_case: test_cases) {
		run_create_sanity_tc(test_case.first, test_case.second.first, test_case.second.second, this);
	}
}