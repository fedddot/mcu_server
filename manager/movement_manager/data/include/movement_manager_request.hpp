#ifndef	MOVEMENT_MANAGER_REQUEST_HPP
#define	MOVEMENT_MANAGER_REQUEST_HPP

#include "movement_manager_vector.hpp"

namespace manager {
	
	struct MovementManagerRequest {
		enum class MovementType: int {
			LINEAR,
			ROTATIONAL,
		};		
		struct LinearMovement {
			Vector<double> destination;
			double speed;
		};
		struct RotationalMovement {
			Vector<double> rotation_center;
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