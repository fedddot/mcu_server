#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "create_gpio_task.hpp"
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

FunctionalParser<Gpio::Direction(const Data&)> gpio_dir_parser(
	[](const Data& cfg)-> Gpio::Direction {
		
	}
)

TEST(ut_create_gpio_task, ctor_dtor_sanity) {
	// GIVEN

	Inventory<int, Gpio> gpio_inventory;
	CreateGpioTask<int> *
}