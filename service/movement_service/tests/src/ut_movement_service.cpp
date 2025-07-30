#include <iostream>
#include <stdexcept>

#include "gtest/gtest.h"

#include "axes_controller.hpp"
#include "axes_controller_config_request.hpp"
#include "linear_movement_request.hpp"
#include "movement_manager.hpp"
#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"
#include "movement_service.hpp"
#include "movement_service_api_response.hpp"
#include "test_axes_controller.hpp"

using namespace service;
using namespace manager;

using AxesConfig = std::string;

static MovementService<AxesConfig>::MovementManagerInstance create_movement_manager(const TestAxesController::Action& action);

TEST(ut_movement_service, ctor_dtor_sanity) {	
	// WHEN
	const auto manager_instance = create_movement_manager(
		[](const Axis&, const Direction&, const double) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	);
	MovementService<AxesConfig> *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(instance_ptr = new MovementService<AxesConfig>(manager_instance));
	ASSERT_NO_THROW(delete instance_ptr);
}

TEST(ut_movement_service, run_api_request_sanity) {
	// GIVEN
	const auto cfg_request = AxesControllerConfigApiRequest<AxesConfig>(AxesConfig("test axes config"));
	const auto test_request = LinearMovementRequest(
		Vector<double>(3.0, 4.0, 5.0),
		4.0
	);
	
	// WHEN
	const auto manager_instance = create_movement_manager(
		[](const Axis& axis, const Direction& direction, const double duration) {
			std::cout << "mocking axes controller is making a step in " << static_cast<int>(direction) << " direction along " << static_cast<int>(axis) << " with duration " << duration << std::endl;
		}
	);
	MovementService<AxesConfig> instance(manager_instance);
	MovementApiResponse response;
	
	// THEN
	ASSERT_NO_THROW(response = instance.run_api_request(cfg_request));
	ASSERT_EQ(MovementApiResponse::Result::SUCCESS, response.result());

	ASSERT_NO_THROW(response = instance.run_api_request(test_request));
	ASSERT_EQ(MovementApiResponse::Result::SUCCESS, response.result());
}

inline MovementService<AxesConfig>::MovementManagerInstance create_movement_manager(const TestAxesController::Action& action) {
	return MovementService<AxesConfig>::MovementManagerInstance(
		new MovementManager<AxesConfig>(
			[action](const AxesConfig& axes_cfg) {
				return manager::Instance<manager::AxesController>(new TestAxesController(action));
			}
		)
	);
}