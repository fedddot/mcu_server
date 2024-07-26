#include <atomic>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "gtest/gtest.h"

#include "buffered_message_receiver.hpp"
#include "custom_creator.hpp"
#include "custom_sender.hpp"
#include "custom_task.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "mcu_server.hpp"
#include "object.hpp"
#include "task.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;
using namespace mcu_platform;
using namespace mcu_platform_utl;

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

TEST(ut_mcu_server, sanity) {
	// GIVEN
	const int expected_result(100);
	Object test_data;
	test_data.add("expected_result", Integer(expected_result));
	CustomCreator<McuTask *(const Data&)> factory(
		[expected_result](const Data& data)-> McuTask * {
			return new CustomTask<Data *(void)>(
				[&data](void) {
					Object report(Data::cast<Object>(data));
					report.add("added_result", Integer(0));
					return report.clone();
				}
			);
		}
	);
	std::atomic<bool> reported(false);
	CustomSender<McuData> sender(
		[expected_result, &reported](const McuData& data) {
			std::cout << "report received: " << std::endl << data << std::endl;
			std::unique_ptr<Data> parsed_report(JsonDataParser().parse(data));
			ASSERT_EQ(expected_result, Data::cast<Integer>(Data::cast<Object>(*parsed_report).access("expected_result")).get());
			ASSERT_EQ(0, Data::cast<Integer>(Data::cast<Object>(*parsed_report).access("added_result")).get());
			reported.store(true, std::memory_order_release);
		}
	);
	const std::string msg_header("msg_header");
	const std::string msg_tail("msg_tail");
	BufferedReceiver receiver(msg_header, msg_tail, 1000UL);
	CustomCreator<Data *(const std::exception&)> failure_report_ctor(
		[](const std::exception&)-> Data * {
			throw std::runtime_error("NOT_IMPLEMENTED");
		}
 	);

	// WHEN
	TestMcuServer instance(
		&sender,
		&receiver,
		JsonDataParser(),
		JsonDataSerializer(),
		factory,
		failure_report_ctor
	);
	const McuData data(JsonDataSerializer().serialize(test_data));
	McuData report("");

	// THEN
	std::thread run_thread(
		[&instance](void) {
			ASSERT_NO_THROW(instance.run());
		}
	);
	receiver.feed(msg_header + data + msg_tail);
	while (!reported.load(std::memory_order_acquire)) {
		sleep(1);
	}
	ASSERT_TRUE(instance.is_running());
	ASSERT_NO_THROW(instance.stop());
	run_thread.join();
	ASSERT_FALSE(instance.is_running());
}