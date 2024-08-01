#include <cstddef>
#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

#include "buffered_custom_ipc_connection.hpp"

using namespace mcu_ipc_utl;

TEST(ut_buffered_custom_ipc_connection, ctor_dtor_sanity) {
	// GIVEN
	const std::string test_head("test_head");
	const std::string test_tail("test_tail");
	const std::size_t test_size(1000UL);
	auto sender = [](const std::string& data) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	BufferedCustomIpcConnection<std::string> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new BufferedCustomIpcConnection<std::string>(test_head, test_tail, test_size, sender)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

TEST(ut_buffered_custom_ipc_connection, feed_sanity) {
	// GIVEN
	const std::string test_head("test_head");
	const std::string test_data("test_data");
	const std::string test_tail("test_tail");
	const std::size_t test_size(1000UL);
	auto sender = [](const std::string& data) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	BufferedCustomIpcConnection<std::string> instance(test_head, test_tail, test_size, sender);
	std::string data("");

	ASSERT_FALSE(instance.readable());
	ASSERT_NO_THROW(instance.feed(test_head));
	ASSERT_FALSE(instance.readable());
	ASSERT_NO_THROW(instance.feed(test_data));
	ASSERT_FALSE(instance.readable());
	ASSERT_NO_THROW(instance.feed(test_tail));
	ASSERT_TRUE(instance.readable());
	ASSERT_NO_THROW(data = instance.read());
	ASSERT_FALSE(instance.readable());
	ASSERT_EQ(data, test_data);
}