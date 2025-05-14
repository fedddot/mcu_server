#include "gtest/gtest.h"

#include "movement_vendor.hpp"

using namespace vendor;

using AxisControllerConfig = std::string;

TEST(ut_movement_vendor, ctor_dtor_sanity) {
	// GIVEN

	// WHEN
	MovementVendor<AxisControllerConfig> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new MovementVendor<AxisControllerConfig>());
	ASSERT_NO_THROW(delete instance_ptr);
}