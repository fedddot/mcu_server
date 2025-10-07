#include "gtest/gtest.h"

#include "motor_drive_service.hpp"
#include "motor_drive_service_api_request.hpp"
#include "motor_drive_service_api_response.hpp"

using namespace service;

using TestStatus = std::string;

TEST(ut_motor_drive_service, run_api_request_sanity) {
	// GIVEN
	const MotorDriveServiceApiRequest test_request(MotorDriveServiceApiRequest::Type::STATUS);

	// WHEN:
	MotorDriveService<TestStatus> service;
	
	// THEN:
	const auto response = service.run_api_request(test_request);
	ASSERT_EQ(response.result(), MotorDriveServiceApiResponse<TestStatus>::Result::SUCCESS);
}