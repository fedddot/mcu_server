#ifndef	LINEAR_MOVEMENT_HPP
#define	LINEAR_MOVEMENT_HPP

#include <stdexcept>
#include <vector>

#include "movement_manager_data.hpp"
#include "movement_manager_vector.hpp"

namespace manager {
	class LinearMovement {
	public:
		LinearMovement(
			const Vector<double>& destination,
			const Vector<double>& basis,
			const double speed
		);
		LinearMovement(const LinearMovement& other) = default;
		LinearMovement& operator=(const LinearMovement&) = default;

		std::vector<Axis> evaluate_steps() const;
	private:
		Vector<double> m_destination;
		Vector<double> m_basis;
		double m_speed;

		static Vector<int> to_discreet_basis(const Vector<double>& destination, const Vector<double>& basis);
		static Vector<double> to_continuous_basis(const Vector<double>& destination, const Vector<double>& basis);

		static Axis longest_axis(const Vector<int>& steps_vector);
	};

	inline LinearMovement::LinearMovement(
		const Vector<double>& destination,
		const Vector<double>& basis,
		const double speed
	): m_destination(destination), m_basis(basis), m_speed(speed) {
		if (m_speed <= 0.0) {
			throw std::invalid_argument("zero or negative speed received");
		}
	}
	
	inline std::vector<Axis> LinearMovement::evaluate_steps() const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // LINEAR_MOVEMENT_HPP