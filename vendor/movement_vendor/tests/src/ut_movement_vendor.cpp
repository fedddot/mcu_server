#include <iostream>
#include <stdexcept>

#include "gtest/gtest.h"

#include "axes_controller.hpp"
#include "axes_controller_config_request.hpp"
#include "linear_movement_request.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"
#include "movement_vendor.hpp"
#include "movement_vendor_api_response.hpp"
#include "test_axes_controller.hpp"

using namespace vendor;
using namespace manager;

using AxesConfig = std::string;

static MovementVendor<AxesConfig>::MovementManagerInstance create_movement_manager(const AxesProperties& axes_properties, const TestAxesController::Action& action);

TEST(ut_movement_vendor, ctor_dtor_sanity) {	
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.2, 0.3);
	
	// WHEN
	const auto manager_instance = create_movement_manager(
		axes_properties,
		[](const AxisStep& step) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	MovementVendor<AxesConfig> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new MovementVendor<AxesConfig>(manager_instance));
	ASSERT_NO_THROW(delete instance_ptr);
}

TEST(ut_movement_vendor, run_api_request_sanity) {
	// GIVEN
	const auto axes_properties = AxesProperties(0.1, 0.2, 0.3);
	const auto cfg_request = AxesControllerConfigApiRequest<AxesConfig>(AxesConfig("test axes config"));
	const auto test_request = LinearMovementRequest(
		Vector<double>(3.0, 4.0, 5.0),
		4.0
	);
	
	// WHEN
	const auto manager_instance = create_movement_manager(
		axes_properties,
		[](const AxisStep& step) {
			std::cout << "mocking axes controller is making a step in " << static_cast<int>(step.direction) << " direction along " << static_cast<int>(step.axis) << " with duration " << step.duration << std::endl;
		}
	);
	MovementVendor<AxesConfig> instance(manager_instance);
	MovementVendorApiResponse response;
	
	// THEN
	ASSERT_NO_THROW(response = instance.run_api_request(cfg_request));
	ASSERT_EQ(MovementVendorApiResponse::Result::SUCCESS, response.result());

	ASSERT_NO_THROW(response = instance.run_api_request(test_request));
	ASSERT_EQ(MovementVendorApiResponse::Result::SUCCESS, response.result());
}

inline MovementVendor<AxesConfig>::MovementManagerInstance create_movement_manager(const AxesProperties& axes_properties, const TestAxesController::Action& action) {
	return MovementVendor<AxesConfig>::MovementManagerInstance(
		new MovementManager<AxesConfig>(
			[action](const AxesConfig& axes_cfg) {
				return manager::Instance<manager::AxesController>(new TestAxesController(action));
			},
			axes_properties
		)
	);
}