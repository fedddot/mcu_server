#include "gtest/gtest.h"
#include <memory>
#include <stdexcept>

#include "create_gpio_task.hpp"
#include "data.hpp"
#include "delete_gpio_task.hpp"
#include "functional_creator.hpp"
#include "functional_parser.hpp"
#include "get_gpio_task.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "set_gpio_task.hpp"
#include "test_gpi.hpp"
#include "test_gpo.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"

using namespace server;
using namespace server_uts;
using namespace engine;
using namespace engine_utl;

Inventory<int, Gpio> s_gpio_inventory;
FunctionalParser<int(const engine::Data&)> s_gpio_id_parser(
	[](const Data& data)-> int {
		return Data::cast<Integer>(Data::cast<Object>(data).access("gpio_id")).get();
	}
);
FunctionalParser<Gpio::Direction(const engine::Data&)> s_gpio_dir_parser(
	[](const Data& data)-> Gpio::Direction {
		return static_cast<Gpio::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_dir")).get());
	}
);
FunctionalParser<Gpio::State(const engine::Data&)> s_gpio_state_parser(
	[](const Data& data)-> Gpio::State {
		return static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(data).access("gpio_state")).get());
	}
);
FunctionalCreator<Gpio *(const int&, const Gpio::Direction&)> s_gpio_creator(
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
const int gpio1_id(0);
const Gpio::Direction gpio1_dir(Gpio::Direction::IN);
const int gpio2_id(1);
const Gpio::Direction gpio2_dir(Gpio::Direction::OUT);

TEST(ut_gpio_tasks, create_sanity) {
	// GIVEN
	Object create_gpio1_task_data;
	create_gpio1_task_data.add("gpio_id", Integer(gpio1_id));
	create_gpio1_task_data.add("gpio_dir", Integer(static_cast<int>(gpio1_dir)));

	Object create_gpio2_task_data;
	create_gpio2_task_data.add("gpio_id", Integer(gpio2_id));
	create_gpio2_task_data.add("gpio_dir", Integer(static_cast<int>(gpio2_dir)));

	// WHEN
	CreateGpioTask<int> create_gpio1_task(
		&s_gpio_inventory,
		create_gpio1_task_data,
		s_gpio_id_parser,
		s_gpio_dir_parser,
		s_gpio_creator
	);
	CreateGpioTask<int> create_gpio2_task(
		&s_gpio_inventory,
		create_gpio2_task_data,
		s_gpio_id_parser,
		s_gpio_dir_parser,
		s_gpio_creator
	);
	std::unique_ptr<Data> report(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(create_gpio1_task.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_TRUE(s_gpio_inventory.contains(gpio1_id));


	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(create_gpio2_task.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_TRUE(s_gpio_inventory.contains(gpio2_id));
}

TEST(ut_gpio_tasks, set_sanity) {
	// GIVEN
	Object set_gpio2_task_data_high;
	set_gpio2_task_data_high.add("gpio_id", Integer(gpio2_id));
	set_gpio2_task_data_high.add("gpio_state", Integer(static_cast<int>(Gpio::State::HIGH)));

	Object set_gpio2_task_data_low;
	set_gpio2_task_data_low.add("gpio_id", Integer(gpio2_id));
	set_gpio2_task_data_low.add("gpio_state", Integer(static_cast<int>(Gpio::State::LOW)));

	// WHEN
	SetGpioTask<int> set_gpio2_task_high(
		&s_gpio_inventory,
		set_gpio2_task_data_high,
		s_gpio_id_parser,
		s_gpio_state_parser
	);
	SetGpioTask<int> set_gpio2_task_low(
		&s_gpio_inventory,
		set_gpio2_task_data_low,
		s_gpio_id_parser,
		s_gpio_state_parser
	);
	std::unique_ptr<Data> report(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(set_gpio2_task_high.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_EQ(Gpio::State::HIGH, Gpio::cast<Gpo>(*(s_gpio_inventory.access(gpio2_id))).state());


	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(set_gpio2_task_low.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_EQ(Gpio::State::LOW, Gpio::cast<Gpo>(*(s_gpio_inventory.access(gpio2_id))).state());
}

TEST(ut_gpio_tasks, get_sanity) {
	// GIVEN
	Object set_gpio2_task_data_high;
	set_gpio2_task_data_high.add("gpio_id", Integer(gpio2_id));
	set_gpio2_task_data_high.add("gpio_state", Integer(static_cast<int>(Gpio::State::HIGH)));

	Object get_gpio1_task_data;
	get_gpio1_task_data.add("gpio_id", Integer(gpio1_id));
	Object get_gpio2_task_data;
	get_gpio2_task_data.add("gpio_id", Integer(gpio2_id));

	// WHEN
	SetGpioTask<int> set_gpio2_task_high(
		&s_gpio_inventory,
		set_gpio2_task_data_high,
		s_gpio_id_parser,
		s_gpio_state_parser
	);
	GetGpioTask<int> get_gpio1_task(
		&s_gpio_inventory,
		get_gpio1_task_data,
		s_gpio_id_parser
	);
	GetGpioTask<int> get_gpio2_task(
		&s_gpio_inventory,
		get_gpio2_task_data,
		s_gpio_id_parser
	);
	std::unique_ptr<Data> report(set_gpio2_task_high.execute());
	report = nullptr;

	// THEN
	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(get_gpio1_task.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_EQ(Gpio::State::LOW, static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(*report).access("gpio_state")).get()));


	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(get_gpio2_task.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_EQ(Gpio::State::HIGH, static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(*report).access("gpio_state")).get()));
}

TEST(ut_gpio_tasks, delete_sanity) {
	// GIVEN
	Object delete_gpio1_task_data;
	delete_gpio1_task_data.add("gpio_id", Integer(gpio1_id));
	Object delete_gpio2_task_data;
	delete_gpio2_task_data.add("gpio_id", Integer(gpio2_id));

	// WHEN
	DeleteGpioTask<int> delete_gpio1_task(
		&s_gpio_inventory,
		delete_gpio1_task_data,
		s_gpio_id_parser
	);
	DeleteGpioTask<int> delete_gpio2_task(
		&s_gpio_inventory,
		delete_gpio2_task_data,
		s_gpio_id_parser
	);
	std::unique_ptr<Data> report(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(delete_gpio1_task.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_FALSE(s_gpio_inventory.contains(gpio1_id));


	ASSERT_NO_THROW(
		(
			report = std::unique_ptr<Data>(delete_gpio2_task.execute())
		)
	);
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
	ASSERT_FALSE(s_gpio_inventory.contains(gpio2_id));
}