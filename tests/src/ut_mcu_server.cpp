#include <exception>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "custom_sender.hpp"
#include "data.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "mcu_server.hpp"
#include "buffered_message_receiver.hpp"
#include "task.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;

using McuData = std::string;
using TestMcuServer = McuServer<McuData>;
using McuTask = Task<Data *(void)>;

TEST(ut_mcu_server, ctor_dtor_sanity) {
	// GIVEN
	CustomSender<McuData> sender(
		[](const McuData&) {
			throw std::runtime_error("NOT_IMPLEMENTED");
		}
	);
	BufferedReceiver receiver("msg_header", "msg_tail", 1000UL);
	CustomCreator<McuTask *(const Data&)> factory(
		[](const Data&)-> McuTask * {
			throw std::runtime_error("NOT_IMPLEMENTED");
		}
 	);
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
				&sender,
				&receiver,
				JsonDataParser(),
				JsonDataSerializer(),
				factory,
				failure_report_ctor
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

// TEST_F(McuServerFixture, feed_sanity) {
// 	// GIVEN
// 	CustomSender<RawData> sender(
// 		[this](const RawData& data) {
// 			std::cout << "received data from server: " << data << std::endl;
// 			std::unique_ptr<Data> report(parser().parse(data));
// 			ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*report).access("result")).get());
// 		}
// 	);

// 	const std::vector<std::pair<std::string, std::string>> test_cases {
// 		{"create_gpio", serializer().serialize(create_gpio_data(1, Gpio::Direction::OUT))},
// 		{"set_gpio", serializer().serialize(set_gpio_data(1, Gpio::State::HIGH))},
// 		{"get_gpio", serializer().serialize(get_gpio_data(1))},
// 		{"delete_gpio", serializer().serialize(delete_gpio_data(1))},
// 		{"delay", serializer().serialize(delay_data(1000))},
// 		{"sequence", serializer().serialize(sequence_data(4))}
// 	};

// 	// WHEN
// 	TestMcuServer instance(
// 		mcu_mcu_server(),
// 		&sender,
// 		receiver(),
// 		parser(),
// 		serializer()
// 	);

// 	// THEN
// 	for (auto test_case: test_cases) {
// 		std::cout << std::endl << std::endl << "running TC: " << test_case.first << std::endl;
// 		std::cout << "feeding data: " << test_case.second << std::endl;
// 		ASSERT_NO_THROW(instance.feed(head() + test_case.second + tail()));
// 	}
// }