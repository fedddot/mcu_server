#include "data.hpp"
#include "functional_creator.hpp"
#include "functional_parser.hpp"
#include "functional_serializer.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "server_task_engine.hpp"
#include "string.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"
#include <exception>
#include <functional>
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

using namespace server;
using namespace server_uts;
using namespace engine;
using namespace engine_utl;

using ServerRawData = std::string;

FunctionalParser<Data *(const ServerRawData&)> s_server_raw_data_parser(
	[](const ServerRawData& data)-> Data * {
		return JsonDataParser().parse(data).clone();
	}
);
FunctionalSerializer<ServerRawData(const Data&)> s_server_raw_data_serializer(
	[](const Data& data)-> ServerRawData {
		return JsonDataSerializer().serialize(Data::cast<Object>(data));
	}
);
FunctionalParser<ServerTaskId(const Data&)> s_server_task_id_parser(
	[](const Data& data)-> ServerTaskId {
		return Data::cast<String>(Data::cast<Object>(data).access("task_id")).get();
	}
);
FunctionalParser<int(const Data&)> s_server_gpio_id_parser(
	[](const Data& data)-> int {
		return Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get();
	}
);
FunctionalParser<Gpio::Direction(const Data&)> s_server_gpio_dir_parser(
	[](const Data& data)-> Gpio::Direction {
		return static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_dir")).get());
	}
);
FunctionalParser<Gpio::State(const Data&)> s_server_gpio_state_parser(
	[](const Data& data)-> Gpio::State {
		return static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
	}
);
FunctionalCreator<Gpio *(const int&, const Gpio::Direction&)> s_server_gpio_creator(
	[](const int& id, const Gpio::Direction& dir) -> Gpio * {
		switch (dir) {
		case Gpio::Direction::IN:
			return new TestGpi();
		case Gpio::Direction::OUT:
			return new TestGpo();
		default:
			throw std::runtime_error("invelid gpio direction");
		}
	}
);
FunctionalCreator<Data *(const std::exception&)> s_server_failure_report_creator(
	[](const std::exception& e) -> Data * {
		Object report;
		report.add("result", Integer(-1));
		report.add("what", String(std::string(e.what())));
		return report.clone();
	}
);
const int gpio1_id(0);
const Gpio::Direction gpio1_dir(Gpio::Direction::IN);
const int gpio2_id(1);
const Gpio::Direction gpio2_dir(Gpio::Direction::OUT);

TEST(ut_server_task_engine, ctor_dtor_sanity) {
	// WHEN
	std::unique_ptr<ServerTaskEngine<ServerRawData, int>> instance_ptr(nullptr);
	
	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = std::unique_ptr<ServerTaskEngine<ServerRawData, int>>(
				new ServerTaskEngine<ServerRawData, int>(
					s_server_task_id_parser,
					s_server_failure_report_creator,
					s_server_raw_data_parser,
					s_server_raw_data_serializer,
					s_server_gpio_id_parser,
					s_server_gpio_dir_parser,
					s_server_gpio_state_parser,
					s_server_gpio_creator
				)
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(instance_ptr = nullptr);
}

TEST(ut_server_task_engine, run_task_sanity) {
	// GIVEN
	const int gpio_id(0);
	const Gpio::Direction gpio_dir(Gpio::Direction::OUT);
	Object create_task_data;
	create_task_data.add("task_id", String("create_gpio"));
	create_task_data.add("gpio_id", Integer(gpio_id));
	create_task_data.add("gpio_dir", Integer(static_cast<int>(gpio_dir)));

	const Gpio::State gpio_state(Gpio::State::HIGH);
	Object set_task_data;
	set_task_data.add("task_id", String("set_gpio"));
	set_task_data.add("gpio_id", Integer(gpio_id));
	set_task_data.add("gpio_state", Integer(static_cast<int>(gpio_state)));

	Object get_task_data;
	get_task_data.add("task_id", String("get_gpio"));
	get_task_data.add("gpio_id", Integer(gpio_id));

	Object delete_task_data;
	delete_task_data.add("task_id", String("delete_gpio"));
	delete_task_data.add("gpio_id", Integer(gpio_id));

	auto regular_result_assertion = [](const Data& data) {
		ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(data).access("result")).get());
	};
	auto get_high_result_assertion = [](const Data& data) {
		ASSERT_EQ(Gpio::State::HIGH, static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get()));
	};
	auto get_low_result_assertion = [](const Data& data) {
		ASSERT_EQ(Gpio::State::LOW, static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get()));
	};

	std::vector<std::pair<Object, std::function<void(const Data&)>>> test_cases {
		{create_task_data, regular_result_assertion},
		{get_task_data, get_low_result_assertion},
		{set_task_data, regular_result_assertion},
		{get_task_data, get_high_result_assertion},
		{delete_task_data, regular_result_assertion}
	};

	// WHEN
	ServerTaskEngine<ServerRawData, int> instance(
		s_server_task_id_parser,
		s_server_failure_report_creator,
		s_server_raw_data_parser,
		s_server_raw_data_serializer,
		s_server_gpio_id_parser,
		s_server_gpio_dir_parser,
		s_server_gpio_state_parser,
		s_server_gpio_creator
	);

	for (auto test_case: test_cases) {
		// WHEN
		ServerRawData report("");

		// THEN
		ServerRawData tc_data(s_server_raw_data_serializer.serialize(test_case.first));
		std::cout << "running data:" << std::endl << tc_data << std::endl;
		ASSERT_NO_THROW(
			report = instance.run_task(tc_data);
		);
		std::cout << "received report:" << std::endl << report << std::endl;
		std::unique_ptr<Data> parsed_report(s_server_raw_data_parser.parse(report));
		test_case.second(*parsed_report);
		
	}
}