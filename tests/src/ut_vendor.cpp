#include "gtest/gtest.h"

#include "vendor.hpp"

using namespace server;

TEST(ut_vendor, ctor_dtor_sanity) {
	// WHEN
	Vendor vendor("test_vendor");
}