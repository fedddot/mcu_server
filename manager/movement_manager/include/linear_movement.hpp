#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <functional>
#include <stdexcept>
#include <tuple>

#include "movement_manager_data.hpp"
#include "vector.hpp"

namespace manager {
	class LinearMovement {
	public:
		LinearMovement(
			const Vector& destination,
			const double speed,
			const double step_length
		);
		LinearMovement(const LinearMovement& other) = default;
		LinearMovement& operator=(const LinearMovement&) = default;

		void perform() const;
	private:
		Vector m_destination;
		double m_speed;
		double m_step_length;

		
		using MovementElement = std::tuple<Axis, int>;
		using MovementElements = std::vector<MovementElement>;

		MovementElements decompose_movement(const Vector& destination) const;
	};

	inline LinearMovement::LinearMovement(
		const Vector& destination,
		const double speed,
		const double step_length
	): m_destination(destination), m_speed(speed), m_step_length(step_length) {
		if (m_destination.zero_vector()) {
			throw std::invalid_argument("zero destination vector received");
		}
		if (m_speed <= 0.0) {
			throw std::invalid_argument("zero or negative speed received");
		}
		if (m_step_length <= 0.0) {
			throw std::invalid_argument("zero or negative step length received");
		}
	}
	
	inline void LinearMovement::perform() const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // LINEAR_MOVEMENT_HPP