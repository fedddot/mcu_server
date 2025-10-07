#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "motor_drive_service.hpp"
#include "motor_drive_service_api_request.hpp"
#include "motor_drive_service_api_response.hpp"
#include "pwm_controller.hpp"

using namespace service;
using namespace provider;

using TestStatus = std::string;

class MockPwmController : public PwmController {
public:
    MOCK_METHOD(void, start, (const double), (override));
	MOCK_METHOD(void, stop, (), (override));
};

TEST(ut_motor_drive_service, run_api_request_sanity) {
	// GIVEN
	const MotorDriveServiceApiRequest test_request(MotorDriveServiceApiRequest::Type::STATUS);

	// WHEN:
	auto pwm_ctrl = testing::NiceMock<MockPwmController>();
	MotorDriveService<TestStatus> service(&pwm_ctrl);
	
	// THEN:
	const auto response = service.run_api_request(test_request);
	ASSERT_EQ(response.result(), MotorDriveServiceApiResponse<TestStatus>::Result::SUCCESS);
}