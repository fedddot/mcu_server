#ifndef	MOTOR_DRIVE_SERVICE_HPP
#define	MOTOR_DRIVE_SERVICE_HPP

#include <stdexcept>

#include "motor_drive_service_api_request.hpp"
#include "motor_drive_service_api_response.hpp"
#include "service.hpp"
#include "pwm_controller.hpp"

namespace service {
	template <typename Status>
	class MotorDriveService: public Service<MotorDriveServiceApiRequest, MotorDriveServiceApiResponse<Status>> {
	public:
		MotorDriveService(
			provider::PwmController *pwm_controller
		): m_pwm_controller(pwm_controller) {
			if (!m_pwm_controller) {
				throw std::invalid_argument("MotorDriveService ctor: invalid arguments");
			}
		}
		MotorDriveService(const MotorDriveService&) = delete;
		MotorDriveService& operator=(const MotorDriveService&) = delete;
		MotorDriveServiceApiResponse<Status> run_api_request(const MotorDriveServiceApiRequest& request) override {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	private:
		provider::PwmController *m_pwm_controller;
	};
}

#endif // MOTOR_DRIVE_SERVICE_HPP