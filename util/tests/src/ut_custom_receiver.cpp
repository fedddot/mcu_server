#include "gtest/gtest.h"
#include <iostream>

#include "custom_listener.hpp"
#include "custom_receiver.hpp"

using namespace mcu_server_utl;

TEST(ut_custom_receiver, ctor_dtor) {
	// WHEN
	CustomReceiver *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new CustomReceiver("test_header", "test_tail")
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

TEST(ut_custom_receiver, feed_sanity) {
	// GIVEN
	const RawData test_header("test_header");
	const RawData test_tail("test_tail");
	const RawData test_data("test data");
	CustomListener<RawData> listener(
		[test_data](const RawData& data) {
			std::cout << "in data listener: received data: " << data << std::endl;
			ASSERT_EQ(test_data, data);
		}
	);

	// WHEN
	CustomReceiver instance(test_header, test_tail);
	instance.set_listener(listener);

	// THEN 
	ASSERT_NO_THROW(instance.feed(test_header));
	ASSERT_NO_THROW(instance.feed(test_data));
	ASSERT_NO_THROW(instance.feed(test_tail));
	ASSERT_NO_THROW(instance.feed(test_header + test_data + test_tail + test_header + test_data + test_tail));
}