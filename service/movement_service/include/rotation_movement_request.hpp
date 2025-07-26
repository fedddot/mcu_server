#ifndef	ROTATION_MOVEMENT_REQUEST_HPP
#define	ROTATION_MOVEMENT_REQUEST_HPP

#include <stdexcept>

#include "movement_manager_vector.hpp"
#include "movement_service_api_request.hpp"

namespace service {
	class RotationMovementRequest: public MovementServiceApiRequest {
	public:
		using Vector = manager::Vector<double>;
		RotationMovementRequest(
			const Vector& destination,
			const Vector& rotation_center,
			const double angle,
			const double speed
		);
		RotationMovementRequest(const RotationMovementRequest&) = default;
		RotationMovementRequest& operator=(const RotationMovementRequest&) = default;
		RequestType type() const override;
		Vector destination() const;
		Vector rotation_center() const;
		double angle() const;
		double speed() const;
	private:
		Vector m_destination;
		Vector m_rotation_center;
		double m_angle;
		double m_speed;
	};

	inline RotationMovementRequest::RotationMovementRequest(
		const Vector& destination,
		const Vector& rotation_center,
		const double angle,
		const double speed
	): m_destination(destination), m_rotation_center(rotation_center), m_angle(angle), m_speed(speed) {
		if (m_speed <= 0.0) {
			throw std::invalid_argument("speed must be greater than 0.0");
		}
	}

	inline MovementServiceApiRequest::RequestType RotationMovementRequest::type() const {
		return RequestType::ROTATIONAL_MOVEMENT;
	}

	inline RotationMovementRequest::Vector RotationMovementRequest::destination() const {
		return m_destination;
	}

	inline RotationMovementRequest::Vector RotationMovementRequest::rotation_center() const {
		return m_rotation_center;
	}

	inline double RotationMovementRequest::angle() const {
		return m_angle;
	}

	inline double RotationMovementRequest::speed() const {
		return m_speed;
	}
}

#endif // ROTATION_MOVEMENT_REQUEST_HPP