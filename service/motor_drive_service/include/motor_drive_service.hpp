#ifndef	MOTOR_DRIVE_SERVICE_HPP
#define	MOTOR_DRIVE_SERVICE_HPP

#include <map>
#include <optional>
#include <stdexcept>

#include "motor_drive_service_api_request.hpp"
#include "motor_drive_service_api_response.hpp"
#include "motor_drive_service_types.hpp"
#include "service.hpp"
#include "pwm_controller.hpp"
#include "digital_output_controller.hpp"

namespace service {
	template <typename Status>
	class MotorDriveService: public Service<MotorDriveServiceApiRequest, MotorDriveServiceApiResponse<Status>> {
	public:
		MotorDriveService(
			provider::PwmController *pwm_controller,
			provider::DigitalOutputController *direction_controller,
			const double max_speed,
			const std::map<Direction, provider::DigitalOutputController::State>& direction_controller_state_mapping
		): m_pwm_controller(pwm_controller), m_direction_controller(direction_controller), m_max_speed(max_speed), m_direction_controller_state_mapping(direction_controller_state_mapping) {
			if (!m_pwm_controller || !m_direction_controller || (m_max_speed <= 0)) {
				throw std::invalid_argument("MotorDriveService ctor: invalid arguments");
			}
			for (const auto& dir: {Direction::CW, Direction::CCW}) {
				if (m_direction_controller_state_mapping.end() == m_direction_controller_state_mapping.find(dir)) {
					throw std::invalid_argument("MotorDriveService ctor: missing state mapping for one of directions");
				}
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
		provider::DigitalOutputController *m_direction_controller;
		const double m_max_speed;
		const std::map<Direction, provider::DigitalOutputController::State> m_direction_controller_state_mapping;
		MotorDriveServiceApiResponse<Status> run_start_request(const MotorDriveServiceApiRequest& request) {
			if (!request.speed().has_value() || !request.direction().has_value()) {
				return MotorDriveServiceApiResponse<Status>(MotorDriveServiceApiResponse<Status>::Result::BAD_REQUEST, std::nullopt);
			}
			if (m_pwm_controller->running()) {
				m_pwm_controller->stop();
			}
			const auto pwm_value = request.speed().value() / m_max_speed;
			const auto direction_control_state = m_direction_controller_state_mapping.at(request.direction().value());
			m_direction_controller->set(direction_control_state);
			m_pwm_controller->start(pwm_value);
			return MotorDriveServiceApiResponse<Status>(MotorDriveServiceApiResponse<Status>::Result::SUCCESS, std::nullopt);
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