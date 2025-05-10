#include "gtest/gtest.h"

#include "axes_controller.hpp"
#include "linear_movement_request.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_response.hpp"
#include "test_axes_controller.hpp"

using namespace manager;

using TestAxesControllerConfig = TestAxesController::Action;

static AxesController *create_axes_ctrlr(const TestAxesControllerConfig& cfg);

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);

	// WHEN
	MovementManager<TestAxesControllerConfig> *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(
		instance = new MovementManager<TestAxesControllerConfig>(
			create_axes_ctrlr,
			axes_properties
		)
	);
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}

TEST(ut_movement_manager, run_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.1, 0.1);
	auto request = LinearMovementRequest(
		Vector<double>(1.0, 2.0, 3.0),
		4.0
	);
	
	// WHEN
	MovementManager<TestAxesControllerConfig> instance(
		create_axes_ctrlr,
		axes_properties
	);
	auto response = MovementManagerResponse {};

	// THEN
	ASSERT_NO_THROW(
		response = instance.run(request);
	);
	ASSERT_EQ(response.code, MovementManagerResponse::ResultCode::OK);
}

inline AxesController *create_axes_ctrlr(const TestAxesControllerConfig& cfg) {
	return new TestAxesController(cfg);
}