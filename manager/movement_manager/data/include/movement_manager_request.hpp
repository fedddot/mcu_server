#ifndef	MOVEMENT_MANAGER_REQUEST_HPP
#define	MOVEMENT_MANAGER_REQUEST_HPP

#include <stdexcept>

#include "movement_manager_vector.hpp"

namespace manager {
	class MovementManagerRequest {
	public:
		enum class RequestType: int {
			INIT,
			UNINIT,
			LINEAR_MOVEMENT,
			ROTATIONAL_MOVEMENT,
		};		
		virtual ~MovementManagerRequest() noexcept = default;
		virtual RequestType type() const = 0;
	};

	class LinearMovementRequest: public MovementManagerRequest {
	public:
		LinearMovementRequest(const Vector<double>& destination, const double speed);
		LinearMovementRequest(const LinearMovementRequest&) = default;
		LinearMovementRequest& operator=(const LinearMovementRequest&) = default;
		RequestType type() const override;
		Vector<double> destination() const;
		double speed() const;
	private:
		Vector<double> m_destination;
		double m_speed;
	};

	inline LinearMovementRequest::LinearMovementRequest(const Vector<double>& destination, const double speed): m_destination(destination), m_speed(speed) {
		if (m_speed <= 0.0) {
			throw std::invalid_argument("speed should be greater than 0.0");
		}
	}

	inline MovementManagerRequest::RequestType LinearMovementRequest::type() const {
		return RequestType::LINEAR_MOVEMENT;
	}

	inline Vector<double> LinearMovementRequest::destination() const {
		return m_destination;
	}

	inline double LinearMovementRequest::speed() const {
		return m_speed;
	}

	class RotationMovementRequest: public MovementManagerRequest {
	public:
		RotationMovementRequest(
			const Vector<double>& destination,
			const Vector<double>& rotation_center,
			const double angle,
			const double speed
		);
		RotationMovementRequest(const RotationMovementRequest&) = default;
		RotationMovementRequest& operator=(const RotationMovementRequest&) = default;
		RequestType type() const override;
		Vector<double> destination() const;
		Vector<double> rotation_center() const;
		double angle() const;
		double speed() const;
	private:
		Vector<double> m_destination;
		Vector<double> m_rotation_center;
		double m_angle;
		double m_speed;
	};

	inline RotationMovementRequest::RotationMovementRequest(
		const Vector<double>& destination,
		const Vector<double>& rotation_center,
		const double angle,
		const double speed
	): m_destination(destination), m_rotation_center(rotation_center), m_angle(angle), m_speed(speed) {
		if (m_speed <= 0.0) {
			throw std::invalid_argument("speed must be greater than 0.0");
		}
	}

	inline MovementManagerRequest::RequestType RotationMovementRequest::type() const {
		return RequestType::ROTATIONAL_MOVEMENT;
	}

	inline Vector<double> RotationMovementRequest::destination() const {
		return m_destination;
	}

	inline Vector<double> RotationMovementRequest::rotation_center() const {
		return m_rotation_center;
	}

	inline double RotationMovementRequest::angle() const {
		return m_angle;
	}

	inline double RotationMovementRequest::speed() const {
		return m_speed;
	}
}

#endif // MOVEMENT_MANAGER_REQUEST_HPP