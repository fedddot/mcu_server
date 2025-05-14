#include "gtest/gtest.h"
#include <stdexcept>

#include "manager.hpp"
#include "manager_instance.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_vendor.hpp"
#include "custom_manager.hpp"

using namespace vendor;
using namespace manager;

using AxisControllerConfig = std::string;

TEST(ut_movement_vendor, ctor_dtor_sanity) {
	// GIVEN
	const auto manager_instance = manager::Instance<Manager<MovementManagerRequest, MovementManagerResponse>>(
		new CustomManager<MovementManagerRequest, MovementManagerResponse>(
			[](const MovementManagerRequest&) -> MovementManagerResponse {
				throw std::runtime_error("NOT IMPLEMENTED");
			}
		)
	);

	// WHEN
	MovementVendor<AxisControllerConfig> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new MovementVendor<AxisControllerConfig>(manager_instance));
	ASSERT_NO_THROW(delete instance_ptr);
}