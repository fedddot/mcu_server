#ifndef	LINEAR_MOVEMENT_REQUEST_HPP
#define	LINEAR_MOVEMENT_REQUEST_HPP

#include <stdexcept>

#include "movement_manager_vector.hpp"
#include "movement_api_request.hpp"

namespace service {
	class LinearMovementRequest: public MovementServiceApiRequest {
	public:
		using Vector = manager::Vector<double>;
		LinearMovementRequest(const Vector& destination, const double speed);
		LinearMovementRequest(const LinearMovementRequest&) = default;
		LinearMovementRequest& operator=(const LinearMovementRequest&) = default;
		RequestType type() const override;
		Vector destination() const;
		double speed() const;
	private:
		Vector m_destination;
		double m_speed;
	};

	inline LinearMovementRequest::LinearMovementRequest(const Vector& destination, const double speed): m_destination(destination), m_speed(speed) {
		if (m_speed <= 0.0) {
			throw std::invalid_argument("speed should be greater than 0.0");
		}
	}

	inline MovementServiceApiRequest::RequestType LinearMovementRequest::type() const {
		return RequestType::LINEAR_MOVEMENT;
	}

	inline LinearMovementRequest::Vector LinearMovementRequest::destination() const {
		return m_destination;
	}

	inline double LinearMovementRequest::speed() const {
		return m_speed;
	}
}

#endif // LINEAR_MOVEMENT_REQUEST_HPP