#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

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
	MockPwmController(): m_running(false) {}
	void start(const double duty_cycle) override {
		(void)(duty_cycle);
		m_running = true;
	}
	void stop() override {
		m_running = false;
	}
	bool running() const override {
		return m_running;
	}
private:
	bool m_running;
};

class MockDigitalOutputController : public DigitalOutputController {
public:
    MOCK_METHOD(void, set, (const DigitalOutputController::State&), (override));
	MOCK_METHOD(DigitalOutputController::State, get, (), (const override));
};

static std::string request_to_str(const MotorDriveServiceApiRequest& request);

TEST(ut_motor_drive_service, run_api_request_sanity) {
	// GIVEN
	const auto max_speed = double(15.0);
	const auto test_speed = double(12.0);
	const auto test_dir = Direction::CCW;
	const auto dir_states_mapping = std::map<Direction, DigitalOutputController::State> {
		{ Direction::CW, DigitalOutputController::State::LOW },
		{ Direction::CCW, DigitalOutputController::State::HIGH },
	};
	const auto test_requests = std::vector<MotorDriveServiceApiRequest> {
		MotorDriveServiceApiRequest(MotorDriveServiceApiRequest::Type::START, test_speed, test_dir),
		MotorDriveServiceApiRequest(MotorDriveServiceApiRequest::Type::STOP),
		MotorDriveServiceApiRequest(MotorDriveServiceApiRequest::Type::STATUS),
	};

	// WHEN:
	auto pwm_ctrl = MockPwmController();
	auto dir_ctrl = testing::NiceMock<MockDigitalOutputController>();
	MotorDriveService<TestStatus> service(&pwm_ctrl, &dir_ctrl, max_speed, dir_states_mapping);
	
	// THEN:
	for (const auto& test_request: test_requests) {
		std::cout << "running test request: " << request_to_str(test_request) << std::endl;
		const auto response = service.run_api_request(test_request);
		ASSERT_EQ(response.result(), MotorDriveServiceApiResponse<TestStatus>::Result::SUCCESS);
	}
}

inline static std::string_view direction_to_str(const Direction& dir) {
	switch (dir) {
	case Direction::CW:
		return "CW";
	case Direction::CCW:
		return "CCW";
	default:
		throw std::invalid_argument("unsupported direction");
	};
}

inline std::string request_to_str(const MotorDriveServiceApiRequest& request) {
	std::stringstream ss;
	ss << "MotorDriveServiceApiRequest of type: ";
	switch (request.type()) {
	case MotorDriveServiceApiRequest::Type::START:
		ss << "START";
		break;
	case MotorDriveServiceApiRequest::Type::STOP:
		ss << "STOP";
		break;
	case MotorDriveServiceApiRequest::Type::STATUS:
		ss << "STATUS";
		break;
	}
	if (request.direction().has_value()) {
		ss << " with direction = " << direction_to_str(request.direction().value());
	}
	if (request.speed().has_value()) {
		ss << " with speed = " << request.speed().value();
	}
	return ss.str();
}