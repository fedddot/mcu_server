#ifndef	MOTOR_DRIVE_CONTROLLER_HPP
#define	MOTOR_DRIVE_CONTROLLER_HPP

#include "motor_drive_service_types.hpp"

namespace service {
	template <typename Status>
	class MotorDriveController {
	public:
		virtual ~MotorDriveController() noexcept = default;

		virtual void set_duty_cycle(const double duty_cycle) = 0;
		virtual void set_direction(const Direction& direction) = 0;

		virtual void enable() = 0;
		virtual bool enabled() const = 0;
		virtual void disable() = 0;
		virtual Status status() const = 0;
	};
}

#endif // MOTOR_DRIVE_CONTROLLER_HPP