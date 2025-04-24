#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <functional>
#include <stdexcept>

#include "vector.hpp"

namespace manager {
	class LinearMovement {
	public:
		using SingleAxisController = std::function<void(const double distance, const double speed)>;
		LinearMovement(
			const Vector& destination,
			const double speed,
			const SingleAxisController& x_controller,
			const SingleAxisController& y_controller,
			const SingleAxisController& z_controller
		);
		LinearMovement(const LinearMovement& other) = default;
		LinearMovement& operator=(const LinearMovement&) = default;

		void perform() const;
	private:
		Vector m_destination;
		double m_speed;
		SingleAxisController m_x_controller;
		SingleAxisController m_y_controller;
		SingleAxisController m_z_controller;
	};

	inline LinearMovement::LinearMovement(
		const Vector& destination,
		const double speed,
		const SingleAxisController& x_controller,
		const SingleAxisController& y_controller,
		const SingleAxisController& z_controller
	): m_destination(destination), m_speed(speed), m_x_controller(x_controller), m_y_controller(y_controller), m_z_controller(z_controller) {
		if (m_speed <= 0.0) {
			throw std::invalid_argument("zero or negative speed received");
		}
		if (m_destination.zero_vector()) {
			throw std::invalid_argument("zero destination vector received");
		}
		if ((m_x_controller == nullptr) || (m_y_controller == nullptr) || (m_z_controller == nullptr)) {
			throw std::invalid_argument("nullptr controller received");
		}
	}
	
	inline void LinearMovement::perform() const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // LINEAR_MOVEMENT_HPP