#include "gtest/gtest.h"

#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_response.hpp"

using namespace manager;

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);

	// WHEN
	MovementManager *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance = new MovementManager(axes_properties)
	);
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}

TEST(ut_movement_manager, run_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);
	const auto request_data = MovementManagerRequest::LinearMovementData {
		.destination = Vector<double>(1.0, 2.0, 3.0),
		.speed = 4.0
	};
	auto request = MovementManagerRequest(request_data);
	
	// WHEN
	MovementManager instance(axes_properties);
	auto response = MovementManagerResponse {};

	// THEN
	ASSERT_NO_THROW(
		response = instance.run(request);
	);
	ASSERT_EQ(response.code, MovementManagerResponse::ResultCode::OK);
}
