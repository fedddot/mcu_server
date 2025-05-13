#include "gtest/gtest.h"

#include "vendor.hpp"

using namespace vendor;

using ApiRequest = std::string;
using ApiResponse = int;

TEST(ut_vendor, ctor_dtor_sanity) {
	// GIVEN

	// WHEN
	Vendor<ApiRequest, ApiResponse> *instance(nullptr);

	// THEN
}