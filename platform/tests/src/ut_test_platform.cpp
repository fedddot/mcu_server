#include "gtest/gtest.h"

#include "test_platform.hpp"

using namespace mcu_platform_uts;

TEST(ut_test_platform, ctor_dtor_sanity) {
	// WHEN
	TestPlatform<int, int> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new TestPlatform<int, int>()
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}