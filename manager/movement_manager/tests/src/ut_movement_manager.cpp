#include <chrono>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <thread>

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
		instance = new MovementManager(
			[](const AxisStep& step, const double step_duration) {
				throw std::runtime_error("NOT IMPLEMENTED");
			},
			axes_properties
		)
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
	MovementManager instance(
		[](const AxisStep& step, const double step_duration) {
			std::cout << "moving along axis " << static_cast<int>(step.axis) << ", in direction " << static_cast<int>(step.direction) << ", step duration " << step_duration << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<std::size_t>(step_duration * 1000)));
		},
		axes_properties
	);
	auto response = MovementManagerResponse {};

	// THEN
	ASSERT_NO_THROW(
		response = instance.run(request);
	);
	ASSERT_EQ(response.code, MovementManagerResponse::ResultCode::OK);
}
