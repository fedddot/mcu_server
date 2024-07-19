#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "custom_sender.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "mcu_server.hpp"
#include "mcu_server_fixture.hpp"
#include "object.hpp"

using namespace engine;
using namespace mcu_task_engine;
using namespace mcu_server;
using namespace mcu_server_uts;
using namespace mcu_server_utl;

using TestMcuServer = McuServer<McuServerFixture::GpioId, McuServerFixture::RawData, McuServerFixture::RawData>;

TEST_F(McuServerFixture, ctor_dtor_sanity) {
	// GIVEN
	CustomSender<RawData> sender(
		[](const RawData&) {

		}
	);

	// WHEN
	TestMcuServer *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestMcuServer(
				mcu_engine(),
				&sender,
				receiver(),
				parser(),
				serializer()
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

TEST_F(McuServerFixture, feed_sanity) {
	// GIVEN
	CustomSender<RawData> sender(
		[this](const RawData& data) {
			std::cout << "received data from server: " << data << std::endl;
			std::unique_ptr<Data> report(parser().parse(data));
			ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
		}
	);

	const std::vector<std::pair<std::string, std::string>> test_cases {
		{"create_gpio", serializer().serialize(create_gpio_data(1, Gpio::Direction::OUT))},
		{"set_gpio", serializer().serialize(set_gpio_data(1, Gpio::State::HIGH))},
		{"get_gpio", serializer().serialize(get_gpio_data(1))},
		{"delete_gpio", serializer().serialize(delete_gpio_data(1))},
		{"sequence", serializer().serialize(sequence_data(4))}
	};

	// WHEN
	TestMcuServer instance(
		mcu_engine(),
		&sender,
		receiver(),
		parser(),
		serializer()
	);

	// THEN
	for (auto test_case: test_cases) {
		std::cout << std::endl << std::endl << "running TC: " << test_case.first << std::endl;
		std::cout << "feeding data: " << test_case.second << std::endl;
		ASSERT_NO_THROW(instance.feed(head() + test_case.second + tail()));
	}
}