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
		MotorDriveService() = default;
		MotorDriveService(const MotorDriveService&) = delete;
		MotorDriveService& operator=(const MotorDriveService&) = delete;
		MotorDriveServiceApiResponse<Status> run_api_request(const MotorDriveServiceApiRequest& request) override {
			throw std::runtime_error("NOT IMPLEMENTED");
		}
	};
}

#endif // MOTOR_DRIVE_SERVICE_HPP