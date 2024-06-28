#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "data.hpp"
#include "functional_connection.hpp"
#include "functional_creator.hpp"
#include "functional_parser.hpp"
#include "functional_serializer.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_task_engine.hpp"
#include "string.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

using namespace server;
using namespace server_uts;
using namespace engine;
using namespace engine_utl;

using RawData = std::string;
using GpioId = int;

using TestServer = Server<RawData, GpioId>;

FunctionalParser<ServerTaskId(const engine::Data&)> s_task_id_parser(
	[](const engine::Data& data) {
		return ServerTaskId(Data::cast<String>(Data::cast<Object>(data).access("task_id")).get());
	}
);

FunctionalCreator<Data *(const std::exception&)> s_failure_report_creator(
	[](const std::exception& e) {
		Object report;
		report.add("result", Integer(-1));
		report.add("what", String(std::string(e.what())));
		return report.clone();
	}
);

FunctionalParser<Data *(const RawData&)> s_raw_data_parser(
	[](const RawData& data)-> Data * {
		return JsonDataParser().parse(data).clone();
	}
);

FunctionalSerializer<RawData(const Data&)> s_raw_data_serializer(
	[](const Data& data)-> RawData {
		return JsonDataSerializer().serialize(Data::cast<Object>(data));
	}
);

FunctionalCreator<Gpio *(const GpioId&, const Gpio::Direction&)> s_gpio_creator(
	[](const GpioId& id, const Gpio::Direction& dir)-> Gpio * {
		switch (dir) {
		case Gpio::Direction::IN:
			return new TestGpi();
		case Gpio::Direction::OUT:
			return new TestGpo();
		default:
			throw std::invalid_argument("unsupported GPIO direction");
		}
	}
);

FunctionalParser<GpioId(const Data&)> s_gpio_id_parser(
	[](const Data& data)-> GpioId {
		return static_cast<GpioId>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_id")).get());
	}
);

FunctionalParser<Gpio::Direction(const Data&)> s_gpio_dir_parser(
	[](const Data& data) {
		return static_cast<Gpio::Direction>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_dir")).get());
	}
);

FunctionalParser<Gpio::State(const Data&)> s_gpio_state_parser(
	[](const Data& data) {
		return static_cast<Gpio::State>(engine::Data::cast<engine::Integer>(engine::Data::cast<engine::Object>(data).access("gpio_state")).get());
	}
);

TEST(ut_server, ctor_dtor_sanity) {
	// GIVEN
	mcu_control_utl::FunctionalConnection<RawData> test_connection(
		[](const RawData& data) {
			(void)(data);
		},
		[]() {
			return false;
		},
		[]() {
			return RawData("");
		} 
	);

	// WHEN
	TestServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestServer(
				&test_connection,
				s_task_id_parser,
				s_failure_report_creator,
				s_raw_data_parser,
				s_raw_data_serializer,
				s_gpio_id_parser,
				s_gpio_dir_parser,
				s_gpio_state_parser,
				s_gpio_creator
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;	
}


TEST(ut_server, run_sanity) {
	// GIVEN
	Object create_gpio_task_data;
	create_gpio_task_data.add("task_id", String("create_gpio"));
	create_gpio_task_data.add("gpio_dir", Integer(static_cast<int>(Gpio::Direction::OUT)));
	create_gpio_task_data.add("gpio_id", Integer(10));

	Object set_gpio_task_data;
	set_gpio_task_data.add("task_id", String("set_gpio"));
	set_gpio_task_data.add("gpio_state", Integer(static_cast<int>(Gpio::State::HIGH)));
	set_gpio_task_data.add("gpio_id", Integer(10));

	Object read_gpio_task_data;
	read_gpio_task_data.add("task_id", String("get_gpio"));
	read_gpio_task_data.add("gpio_id", Integer(10));

	Object delete_gpio_task_data;
	delete_gpio_task_data.add("task_id", String("delete_gpio"));
	delete_gpio_task_data.add("gpio_id", Integer(10));

	const std::map<std::string, RawData> test_cases {
		{"create_gpio",		s_raw_data_serializer.serialize(create_gpio_task_data)},
		{"set_gpio",		s_raw_data_serializer.serialize(set_gpio_task_data)},
		{"read_gpio",		s_raw_data_serializer.serialize(read_gpio_task_data)},
		{"create_gpio",		s_raw_data_serializer.serialize(delete_gpio_task_data)},
	};

	// WHEN
	auto test_cases_iter = test_cases.begin();
	mcu_control_utl::FunctionalConnection<RawData> test_connection(
		[this](const RawData& data) {
			std::cout << "server sends report:" << std::endl << data << std::endl;
			std::unique_ptr<Data> report_parsed(s_raw_data_parser.parse(data));
			ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report_parsed).access("result")).get());
		},
		[&test_cases_iter, &test_cases]() {
			return test_cases_iter != test_cases.end();
		},
		[&test_cases_iter]() {
			auto data(test_cases_iter->second);
			std::cout << "server reads data:" << std::endl << data << std::endl;
			++test_cases_iter;
			return data;
		}
	);
	TestServer instance(
		&test_connection,
		s_task_id_parser,
		s_failure_report_creator,
		s_raw_data_parser,
		s_raw_data_serializer,
		s_gpio_id_parser,
		s_gpio_dir_parser,
		s_gpio_state_parser,
		s_gpio_creator
	);

	// THEN
	std::thread run_thread(
		[](TestServer *server) {
			ASSERT_NO_THROW(server->run());
		},
		&instance
	);
	sleep(1);
	ASSERT_TRUE(instance.is_running());
	ASSERT_NO_THROW(instance.stop());
	run_thread.join();
}