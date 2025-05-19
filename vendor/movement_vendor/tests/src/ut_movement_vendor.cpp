#include <optional>
#include <stdexcept>

#include "gtest/gtest.h"

#include "custom_manager.hpp"
#include "linear_movement_request.hpp"
#include "manager.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_manager_vector.hpp"
#include "movement_vendor.hpp"
#include "movement_vendor_api_request.hpp"
#include "vendor_instance.hpp"

using namespace vendor;
using namespace manager;

using AxesConfig = std::string;

TEST(ut_movement_vendor, ctor_dtor_sanity) {
	// GIVEN
	const auto manager_instance = vendor::Instance<Manager<MovementManagerRequest, MovementManagerResponse>>(
		new CustomManager<MovementManagerRequest, MovementManagerResponse>(
			[](const MovementManagerRequest&) -> MovementManagerResponse {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);

	// WHEN
	MovementVendor<AxesConfig> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new MovementVendor<AxesConfig>(manager_instance));
	ASSERT_NO_THROW(delete instance_ptr);
}

TEST(ut_movement_vendor, run_api_request_sanity) {
	// GIVEN
	const auto manager_instance = vendor::Instance<Manager<MovementManagerRequest, MovementManagerResponse>>(
		new CustomManager<MovementManagerRequest, MovementManagerResponse>(
			[](const MovementManagerRequest&) -> MovementManagerResponse {
				return MovementManagerResponse { .code = MovementManagerResponse::ResultCode::OK, .message = std::nullopt };
			}
		)
	);
	const auto test_request = MovementVendorApiRequest(
		new LinearMovementRequest(
			Vector<double>(0.0, 0.0, 0.0),
			4.0
		)
	);

	// WHEN
	MovementVendor<AxesConfig> instance(manager_instance);

	// THEN
	ASSERT_NO_THROW(
		{
			const auto response = instance.run_api_request(test_request);
		}
	);
}