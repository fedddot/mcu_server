#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "gpio.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_creator.hpp"
#include "server_parser.hpp"
#include "server_serializer.hpp"

#include "test_gpi.hpp"
#include "test_gpo.hpp"

#include "test_case.hpp"

using namespace server;
using namespace server_ut;
using namespace server_utl;

ServerParser<Data *(const TestCase::RawData&)> server_ut::TestCase::s_raw_data_parser(
	[](const RawData& data)-> Data * {
		return JsonDataParser().parse(data).clone();
	}
);
ServerSerializer<TestCase::RawData(const Data&)> TestCase::s_raw_data_serializer(
	[](const Data& data)-> TestCase::RawData {
		return JsonDataSerializer().serialize(Data::cast<Object>(data));
	}
);
ServerCreator<Gpio *(const TestCase::GpioId&, const Gpio::Direction&)> TestCase::s_gpio_creator(
	[](const GpioId& id, const Gpio::Direction& dir)-> Gpio * {
		(void)id;
		switch (dir) {
		case Gpio::Direction::IN:
			return new TestGpi();
		case Gpio::Direction::OUT:
			return new TestGpo();
		default:
			throw std::invalid_argument("not supported");
		}
	}
);
std::unique_ptr<Server<TestCase::RawData, TestCase::GpioId>> TestCase::s_instance(
	new Server<TestCase::RawData, TestCase::GpioId>(
		s_raw_data_parser,
		s_raw_data_serializer,
		s_gpio_creator
	)
);