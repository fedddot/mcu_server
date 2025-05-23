#ifndef	MOVEMENT_MANAGER_DATA_HPP
#define	MOVEMENT_MANAGER_DATA_HPP

#include <stdexcept>

namespace manager {
	enum class Axis: int {
		X = 0,
		Y = 1,
		Z = 2,
	};

	enum class Direction: int {
		NEGATIVE = 0,
		POSITIVE = 1,
	};

	class AxisStep {
	public:
		AxisStep(const Axis& axis = Axis::X, const Direction& direction = Direction::POSITIVE, const double duration = 1.0);
		AxisStep(const AxisStep&) = default;
		AxisStep& operator=(const AxisStep&) = default;
		virtual ~AxisStep() noexcept = default;

		Axis axis() const;
		Direction direction() const;
		double duration() const;
	private:
		Axis m_axis;
		Direction m_direction;
		double m_duration;
	};

	inline AxisStep::AxisStep(const Axis& axis, const Direction& direction, const double duration): m_axis(axis), m_direction(direction), m_duration(duration) {
		if (duration <= 0.0) {
			throw std::invalid_argument("duration must be a positive non-zero value");
		}
	}

	inline Axis AxisStep::axis() const {
		return m_axis;
	}

	inline Direction AxisStep::direction() const {
		return m_direction;
	}

	inline double AxisStep::duration() const {
		return m_duration;
	}
}

#endif // MOVEMENT_MANAGER_DATA_HPP