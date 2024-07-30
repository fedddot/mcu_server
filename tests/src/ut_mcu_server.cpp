#include <condition_variable>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "mcu_server.hpp"
#include "object.hpp"
#include "task.hpp"

#include "mcu_server_fixture.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;
using namespace mcu_server_uts;
using namespace mcu_platform;
using namespace mcu_platform_utl;

using McuData = std::string;
using TestMcuServer = McuServer<McuData>;
using McuTask = Task<Data *(void)>;

TEST_F(McuServerFixture, ctor_dtor_sanity) {
	// GIVEN
	CustomCreator<Data *(const std::exception&)> failure_report_ctor(
		[](const std::exception&)-> Data * {
			throw std::runtime_error("NOT_IMPLEMENTED");
		}
 	);

	// WHEN
	TestMcuServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestMcuServer(
				platform()->message_sender(),
				platform()->message_receiver(),
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

	std::mutex mux;
	std::condition_variable cond;
	McuServerFixture::McuData report("");

	// WHEN
	fixture->set_sender(
		[&mux, &cond, &report](const McuServerFixture::McuData& data) {
			std::unique_lock lock(mux);
			std::cout << "mcu server sends data: " << data << std::endl;
			report = data;
			cond.notify_all();			
		}
	);
	TestMcuServer instance(
		fixture->platform()->message_sender(),
		fixture->platform()->message_receiver(),
		fixture->parser(),
		fixture->serializer(),
		fixture->factory(),
		fixture->fail_report_creator()
	);

	std::thread run_thread(
		[](TestMcuServer *instance) {
			instance->run();
		},
		&instance
	);
	
	ASSERT_NO_THROW(fixture->platform()->feed_receiver(fixture->msg_head()));
	ASSERT_NO_THROW(fixture->platform()->feed_receiver(test_data));
	ASSERT_NO_THROW(fixture->platform()->feed_receiver(fixture->msg_tail()));

	while (true) {
		std::unique_lock lock(mux);
		if (!report.empty()) {
			break;
		}
		cond.wait(lock);	
	}

	// THEN
	ASSERT_TRUE(instance.is_running());
	ASSERT_NO_THROW(instance.stop());
	run_thread.join();
	ASSERT_FALSE(instance.is_running());
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