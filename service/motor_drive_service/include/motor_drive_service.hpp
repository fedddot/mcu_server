#ifndef	MOTOR_DRIVE_SERVICE_HPP
#define	MOTOR_DRIVE_SERVICE_HPP

#include <optional>
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
			switch (request.type()) {
			case service::MotorDriveServiceApiRequest::Type::START:
				return run_start_request(request);
			case service::MotorDriveServiceApiRequest::Type::STOP:
				return run_stop_request(request);
			case service::MotorDriveServiceApiRequest::Type::STATUS:
				return run_get_request(request);
			default:
				return MotorDriveServiceApiResponse<Status>(MotorDriveServiceApiResponse<Status>::Result::BAD_REQUEST, std::nullopt);
			}
		}
	private:
		provider::PwmController *m_pwm_controller;

		MotorDriveServiceApiResponse<Status> run_start_request(const MotorDriveServiceApiRequest& request) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
		MotorDriveServiceApiResponse<Status> run_stop_request(const MotorDriveServiceApiRequest& request) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
		MotorDriveServiceApiResponse<Status> run_get_request(const MotorDriveServiceApiRequest& request) {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	};
}

#endif // MOTOR_DRIVE_SERVICE_HPP