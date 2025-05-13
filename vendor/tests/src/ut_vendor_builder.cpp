#include "gtest/gtest.h"

#include "vendor_builder.hpp"

using namespace vendor;

using ApiRequest = std::string;
using ApiResponse = int;

TEST(ut_vendor_builder, ctor_dtor_sanity) {
	// GIVEN

	// WHEN
	VendorBuilder<ApiRequest, ApiResponse> *instance(nullptr);

	// THEN
}