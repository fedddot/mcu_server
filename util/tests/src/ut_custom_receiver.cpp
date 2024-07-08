#include "gtest/gtest.h"

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