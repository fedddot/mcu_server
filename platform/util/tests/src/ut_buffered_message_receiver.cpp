#include <cstddef>
#include <string>

#include "gtest/gtest.h"

#include "buffered_message_receiver.hpp"

using namespace mcu_platform;
using namespace mcu_platform_utl;

TEST(ut_buffered_message_receiver, ctor_dtor) {
	// WHEN
	BufferedReceiver *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new BufferedReceiver("test_header", "test_tail", 1000UL)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

TEST(ut_buffered_message_receiver, sanity) {
	// GIVEN
	const std::string test_header("test_header");
	const std::string test_tail("test_tail");
	const std::string test_data("test data");
	const std::size_t max_msg_size(1000UL);

	// WHEN
	BufferedReceiver instance(test_header, test_tail, max_msg_size);
	std::string data("");

	// THEN
	ASSERT_FALSE(instance.message_received());
	ASSERT_NO_THROW(instance.feed(test_header));
	ASSERT_FALSE(instance.message_received());
	ASSERT_NO_THROW(instance.feed(test_data));
	ASSERT_FALSE(instance.message_received());
	ASSERT_NO_THROW(instance.feed(test_tail));
	ASSERT_TRUE(instance.message_received());
	ASSERT_NO_THROW(data = instance.receive());
	ASSERT_FALSE(instance.message_received());
	ASSERT_EQ(test_data, data);
}