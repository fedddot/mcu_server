#include <stdexcept>
#include <thread>

#include "gtest/gtest.h"

#include "axes_controller.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "test_axes_controller.hpp"

using namespace manager;

using TestAxesControllerConfig = TestAxesController::Action;

static AxesController *create_axes_ctrlr(const TestAxesControllerConfig& cfg);

TEST(ut_movement_manager, ctor_dtor_sanity) {
	// WHEN
	MovementManager<TestAxesControllerConfig> *instance(nullptr);

	// THEN
	ASSERT_NO_THROW(instance = new MovementManager<TestAxesControllerConfig>(create_axes_ctrlr));
	ASSERT_NE(instance, nullptr);
	ASSERT_NO_THROW(delete instance);
}

TEST(ut_movement_manager, init_sanity) {
	// GIVEN
	const auto test_controller_config_data = [](const Axis&, const Direction&, const double) {
		throw std::runtime_error("NOT IMPLEMENTED");
	};

	// WHEN
	MovementManager<TestAxesControllerConfig> instance(create_axes_ctrlr);

	// THEN
	ASSERT_NO_THROW(instance.init(test_controller_config_data));
}

TEST(ut_movement_manager, linear_movement_sanity) {
	// GIVEN
	const auto test_destination = Vector<double>(1.0, 2.0, 3.0);
	const auto test_speed = 4.0;
	const auto test_controller_config_data = [](const Axis& axis, const Direction& direction, const double duration) {
		std::cout << "moving along axis " << static_cast<int>(axis) << ", in direction " << static_cast<int>(direction) << ", step duration " << duration << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<std::size_t>(duration * 1000)));
	};
	
	// WHEN
	MovementManager<TestAxesControllerConfig> instance(create_axes_ctrlr);
	instance.init(test_controller_config_data);
	
	// THEN
	ASSERT_NO_THROW(instance.linear_movement(test_destination, test_speed));
}

inline AxesController *create_axes_ctrlr(const TestAxesControllerConfig& cfg) {
	return new TestAxesController(cfg);
}