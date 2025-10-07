#ifndef	MOTOR_DRIVE_SERVICE_HPP
#define	MOTOR_DRIVE_SERVICE_HPP

#include <optional>
#include <stdexcept>

#include "motor_drive_controller.hpp"
#include "motor_drive_service_api_request.hpp"
#include "motor_drive_service_api_response.hpp"
#include "service.hpp"

namespace service {
	template <typename Status>
	class MotorDriveService: public Service<MotorDriveServiceApiRequest, MotorDriveServiceApiResponse<Status>> {
	public:
		MotorDriveService(
			MotorDriveController<Status> *motor_drive_controller,
			const double max_speed
		): m_motor_drive_controller(motor_drive_controller), m_max_speed(max_speed) {
			if (!m_motor_drive_controller || (m_max_speed <= 0)) {
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
		MotorDriveController<Status> *m_motor_drive_controller;
		const double m_max_speed;
		MotorDriveServiceApiResponse<Status> run_start_request(const MotorDriveServiceApiRequest& request) {
			if (!request.speed().has_value() || !request.direction().has_value()) {
				return MotorDriveServiceApiResponse<Status>(MotorDriveServiceApiResponse<Status>::Result::BAD_REQUEST, std::nullopt);
			}
			if (m_motor_drive_controller->enabled()) {
				m_motor_drive_controller->disable();
			}
			m_motor_drive_controller->set_duty_cycle(request.speed().value() / m_max_speed);
			m_motor_drive_controller->set_direction(request.direction().value());
			m_motor_drive_controller->enable();
			return MotorDriveServiceApiResponse<Status>(MotorDriveServiceApiResponse<Status>::Result::SUCCESS, std::nullopt);
		}
		MotorDriveServiceApiResponse<Status> run_stop_request(const MotorDriveServiceApiRequest& request) {
			if (m_motor_drive_controller->enabled()) {
				m_motor_drive_controller->disable();
			}
			return MotorDriveServiceApiResponse<Status>(MotorDriveServiceApiResponse<Status>::Result::SUCCESS, std::nullopt);
		}
		MotorDriveServiceApiResponse<Status> run_get_request(const MotorDriveServiceApiRequest& request) {
			return MotorDriveServiceApiResponse<Status>(MotorDriveServiceApiResponse<Status>::Result::FAILURE, std::nullopt);
		}
	};
}

#endif // MOTOR_DRIVE_SERVICE_HPP