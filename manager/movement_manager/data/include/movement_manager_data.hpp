#ifndef	MOVEMENT_MANAGER_DATA_HPP
#define	MOVEMENT_MANAGER_DATA_HPP

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
		Axis axis;
		Direction direction;
	};
}

#endif // MOVEMENT_MANAGER_DATA_HPP