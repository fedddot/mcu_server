#ifndef	MOTOR_DRIVE_SERVICE_API_REQUEST_HPP
#define	MOTOR_DRIVE_SERVICE_API_REQUEST_HPP

#include <functional>
#include <optional>

#include "motor_drive_service_types.hpp"

namespace service {
	class MotorDriveServiceApiRequest {
	public:
		enum class Type: int {
			START,
			STOP,
			STATUS
		};
		MotorDriveServiceApiRequest(
			const Type& type,
			const std::optional<double>& speed = std::nullopt,
			const std::optional<Direction>& direction = std::nullopt
		): m_type(type), m_speed(speed), m_direction(direction) {}
		MotorDriveServiceApiRequest(const MotorDriveServiceApiRequest&) = default;
		MotorDriveServiceApiRequest& operator=(const MotorDriveServiceApiRequest&) = default;
		virtual ~MotorDriveServiceApiRequest() noexcept = default;
		const Type& type() const { return std::ref(m_type); }
		const std::optional<double>& speed() const { return std::ref(m_speed); }
		const std::optional<Direction>& direction() const { return std::ref(m_direction); }
	private:
		Type m_type;
		std::optional<double> m_speed;
		std::optional<Direction> m_direction;
	};
}

#endif // MOTOR_DRIVE_SERVICE_API_REQUEST_HPP