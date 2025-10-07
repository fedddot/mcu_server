#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <map>

#include "digital_output_controller.hpp"
#include "motor_drive_service.hpp"
#include "motor_drive_service_api_request.hpp"
#include "motor_drive_service_api_response.hpp"
#include "motor_drive_service_types.hpp"
#include "pwm_controller.hpp"

using namespace service;
using namespace provider;

using TestStatus = std::string;

class MockPwmController : public PwmController {
public:
    MOCK_METHOD(void, start, (const double), (override));
	MOCK_METHOD(void, stop, (), (override));
	MOCK_METHOD(bool, running, (), (const override));
};

class MockDigitalOutputController : public DigitalOutputController {
public:
    MOCK_METHOD(void, set, (const DigitalOutputController::State&), (override));
	MOCK_METHOD(DigitalOutputController::State, get, (), (const override));
};

TEST(ut_motor_drive_service, run_api_request_sanity) {
	// GIVEN
	const auto max_speed = (double)(15.0);
	const auto test_speed = (double)(12.0);
	const auto test_dir = Direction::CCW;
	const auto dir_states_mapping = std::map<Direction, DigitalOutputController::State> {
		{ Direction::CW, DigitalOutputController::State::LOW },
		{ Direction::CCW, DigitalOutputController::State::HIGH },
	};
	const MotorDriveServiceApiRequest test_request(MotorDriveServiceApiRequest::Type::START, test_speed, test_dir);

	// WHEN:
	auto pwm_ctrl = testing::NiceMock<MockPwmController>();
	auto dir_ctrl = testing::NiceMock<MockDigitalOutputController>();
	MotorDriveService<TestStatus> service(&pwm_ctrl, &dir_ctrl, max_speed, dir_states_mapping);
	
	// THEN:
	const auto response = service.run_api_request(test_request);
	ASSERT_EQ(response.result(), MotorDriveServiceApiResponse<TestStatus>::Result::SUCCESS);
}