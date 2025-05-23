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

	struct AxisStep {
		AxisStep(const Axis& axis = Axis::X, const Direction& direction = Direction::POSITIVE, const double duration = 1.0);
		AxisStep(const AxisStep&) = default;
		AxisStep& operator=(const AxisStep&) = default;
		virtual ~AxisStep() noexcept = default;

		Axis axis;
		Direction direction;
		double duration;
	};

	inline AxisStep::AxisStep(const Axis& axis, const Direction& direction, const double duration): axis(axis), direction(direction), duration(duration) {
		if (duration <= 0.0) {
			throw std::invalid_argument("duration must be a positive non-zero value");
		}
	}
}

#endif // MOVEMENT_MANAGER_DATA_HPP