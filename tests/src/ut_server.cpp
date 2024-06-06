#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "data.hpp"
#include "gpio.hpp"
#include "server_creator.hpp"
#include "server_parser.hpp"
#include "server_serializer.hpp"
#include "test_case.hpp"

#include "server.hpp"

using namespace server;
using namespace server_ut;
using namespace server_utl;

using RawData = std::string;
using GpioId = int;

TEST(ut_server, ctor_dtor_sanity) {
	// GIVEN
	ServerCreator<Gpio *(const GpioId&, const Gpio::Direction&)> gpio_creator(
		[](const GpioId& id, const Gpio::Direction& dir)-> Gpio * {
			throw std::runtime_error("not implemented");
		}
	);
	ServerParser<Data *(const RawData&)> raw_data_parser(
		[](const RawData& data)-> Data * {
			throw std::runtime_error("not implemented");
		}
	);
	ServerSerializer<RawData(const Data&)> raw_data_serializer(
		[](const Data& data)-> RawData {
			throw std::runtime_error("not implemented");
		}
	);

	// WHEN
	Server<RawData, GpioId> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new Server<RawData, GpioId>(
				raw_data_parser,
				raw_data_serializer,
				gpio_creator
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;	
}

TEST(ut_server, run_task_sanity) {
	// GIVEN
	const std::vector<TestCase> test_cases {
		TestCase(
			"create valid gpi", 
			"{\"task_id\": \"create_gpio\", \"gpio_id\": 0, \"gpio_dir\": 0}", 
			0
		),
		TestCase(
			"create valid gpo", 
			"{\"task_id\": \"create_gpio\", \"gpio_id\": 1, \"gpio_dir\": 1}", 
			0
		),
		TestCase(
			"set created gpo to HIGH state", 
			"{\"task_id\": \"set_gpio\", \"gpio_id\": 1, \"gpio_state\": 1}", 
			0
		),
		TestCase(
			"set created gpo to LOW state", 
			"{\"task_id\": \"set_gpio\", \"gpio_id\": 1, \"gpio_state\": 0}", 
			0
		),
		TestCase(
			"delete gpio", 
			"{\"task_id\": \"delete_gpio\", \"gpio_id\": 1}", 
			0
		),
		TestCase(
			"delete gpio", 
			"{\"task_id\": \"delete_gpio\", \"gpio_id\": 0}", 
			0
		)
	};

	// WHEN
	TestCase::reset_instance();

	// THEN
	for (auto test_case: test_cases) {
		test_case.run();
	}
}

TEST(ut_server, run_task_negative_wrong_cfg) {
	// GIVEN
	const std::vector<TestCase> test_cases {
		TestCase(
			"missing \"task_id\" field", 
			"{\"gpio_id\": 0, \"gpio_dir\": 0}", 
			-1
		),
		TestCase(
			"wrong \"task_id\" field type", 
			"{\"task_id\": 0, \"gpio_id\": 1, \"gpio_dir\": 1}", 
			-1
		),
		TestCase(
			"missing \"gpio_id\" field", 
			"{\"task_id\": \"create_gpio\", \"gpio_dir\": 1}", 
			-1
		),
		TestCase(
			"missing \"gpio_dir\" field", 
			"{\"task_id\": \"create_gpio\", \"gpio_id\": 1}", 
			-1
		),
		TestCase(
			"deleting not existing field", 
			"{\"task_id\": \"delete_gpio\", \"gpio_id\": 1024}", 
			-1
		)
	};

	// WHEN
	TestCase::reset_instance();

	// THEN
	for (auto test_case: test_cases) {
		test_case.run();
	}
}