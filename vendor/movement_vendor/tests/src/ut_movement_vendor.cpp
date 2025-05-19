#include <stdexcept>

#include "gtest/gtest.h"

#include "linear_movement_request.hpp"
#include "movement_manager_vector.hpp"
#include "movement_vendor.hpp"
#include "movement_vendor_api_response.hpp"

using namespace vendor;
using namespace manager;

using AxesConfig = std::string;

static MovementVendor<AxesConfig>::MovementManagerInstance create_movement_manager();

TEST(ut_movement_vendor, ctor_dtor_sanity) {	
	// WHEN
	const auto manager_instance = create_movement_manager();
	MovementVendor<AxesConfig> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new MovementVendor<AxesConfig>(manager_instance));
	ASSERT_NO_THROW(delete instance_ptr);
}

TEST(ut_movement_vendor, run_api_request_sanity) {
	// GIVEN
	const auto test_request = LinearMovementRequest(
		Vector<double>(0.0, 0.0, 0.0),
		4.0
	);
	
	// WHEN
	const auto manager_instance = create_movement_manager();
	MovementVendor<AxesConfig> instance(manager_instance);
	MovementVendorApiResponse response;

	// THEN
	ASSERT_NO_THROW(
		{
			response = instance.run_api_request(test_request);
		}
	);
	ASSERT_EQ(MovementVendorApiResponse::Result::SUCCESS, response.result());
}

MovementVendor<AxesConfig>::MovementManagerInstance create_movement_manager() {
	throw std::runtime_error("NOT IMPLEMENTED");
}