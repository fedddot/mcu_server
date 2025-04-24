#ifndef	MOVEMENT_MANAGER_REQUEST_HPP
#define	MOVEMENT_MANAGER_REQUEST_HPP

#include "vector.hpp"

namespace manager {
	
	struct MovementManagerRequest {
		enum class MovementType: int {
			LINEAR,
			ROTATIONAL,
		};		
		struct LinearMovement {
			Vector destination;
			double speed;
		};
		struct RotationalMovement {
			Vector rotation_center;
			double angle;
			double speed;
		};
		
		MovementType type;
		union {
			LinearMovement linear;
			RotationalMovement rotational;
		} movement;
	};
}

#endif // MOVEMENT_MANAGER_REQUEST_HPP