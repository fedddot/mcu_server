#include "serial_connection.hpp"
#include "gtest/gtest.h"
#include <memory>

using namespace mcu_control_utl;

TEST(ut_serial_connection, ctor_dtor_sanity) {
	// WHEN
	SerialConnection *instance_ptr(nullptr);
	
	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new SerialConnection(
				[](const SerialData& data) {
					(void)(data);
				},
				"abc",
				"def"
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);
	instance_ptr = nullptr;
}

TEST(ut_serial_connection, feed_sanity) {
	// WHEN
	SerialData head("abc");
	SerialData tail("efg");
	SerialData data("test data");
	SerialConnection instance(
		[](const SerialData& data) {
			(void)(data);
		},
		head,
		tail
	);
	SerialData read_data("");
	
	// THEN
	ASSERT_NO_THROW(instance.feed_data(head));
	ASSERT_FALSE(instance.is_readable());
	ASSERT_NO_THROW(instance.feed_data(data));
	ASSERT_FALSE(instance.is_readable());
	ASSERT_NO_THROW(instance.feed_data(tail));
	ASSERT_TRUE(instance.is_readable());
	ASSERT_NO_THROW(read_data = instance.read_data());
	ASSERT_EQ(data, read_data);
}