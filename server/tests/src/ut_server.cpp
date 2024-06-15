#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "gpio.hpp"
#include "object.hpp"
#include "integer.hpp"

#include "functional_creator.hpp"

#include "functional_parser.hpp"
#include "json_data_parser.hpp"
#include "functional_serializer.hpp"
#include "json_data_serializer.hpp"

#include "server.hpp"
#include "server_tasks.hpp"
#include "string.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"

using namespace server;
using namespace server_utl;
using namespace server_uts;
using namespace engine;
using namespace engine_utl;

using RawData = std::string;
using GpioId = int;

using TestServer = Server<RawData, GpioId>;

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

TEST(ut_server, ctor_dtor_sanity) {
	// WHEN
	TestServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestServer(
				s_raw_data_parser,
				s_raw_data_serializer,
				s_gpio_creator,
				s_gpio_id_parser
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
	create_gpio_task_data.add("task_id", Integer(static_cast<int>(TaskId::CREATE_GPIO)));
	create_gpio_task_data.add("gpio_dir", Integer(static_cast<int>(Gpio::Direction::OUT)));
	create_gpio_task_data.add("gpio_id", Integer(10));

	Object set_gpio_task_data;
	set_gpio_task_data.add("task_id", Integer(static_cast<int>(TaskId::SET_GPIO_STATE)));
	set_gpio_task_data.add("gpio_state", Integer(static_cast<int>(Gpio::State::HIGH)));
	set_gpio_task_data.add("gpio_id", Integer(10));

	Object read_gpio_task_data;
	read_gpio_task_data.add("task_id", Integer(static_cast<int>(TaskId::READ_GPIO_STATE)));
	read_gpio_task_data.add("gpio_id", Integer(10));

	Object delete_gpio_task_data;
	delete_gpio_task_data.add("task_id", Integer(static_cast<int>(TaskId::DELETE_GPIO)));
	delete_gpio_task_data.add("gpio_id", Integer(10));

	const std::map<std::string, RawData> test_cases {
		{"create_gpio",		s_raw_data_serializer.serialize(create_gpio_task_data)},
		{"set_gpio",		s_raw_data_serializer.serialize(set_gpio_task_data)},
		{"read_gpio",		s_raw_data_serializer.serialize(read_gpio_task_data)},
		{"create_gpio",		s_raw_data_serializer.serialize(delete_gpio_task_data)},
	};

	// WHEN
	TestServer instance(
		s_raw_data_parser,
		s_raw_data_serializer,
		s_gpio_creator,
		s_gpio_id_parser
	);

	for (auto test_case: test_cases) {
		// WHEN
		RawData result("");

		// THEN
		std::cout << "TC: " << test_case.first << std::endl;
		std::cout << "task data:" << std::endl << test_case.second << std::endl;
		
		ASSERT_NO_THROW(result = instance.run(test_case.second));
		
		std::cout << "task report:" << std::endl << result << std::endl;
		
		ASSERT_EQ(0, Data::cast<Integer>(JsonDataParser().parse(result).access("result")).get());
	}
}