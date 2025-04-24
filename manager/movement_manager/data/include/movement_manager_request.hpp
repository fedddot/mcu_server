#ifndef	MOVEMENT_MANAGER_REQUEST_HPP
#define	MOVEMENT_MANAGER_REQUEST_HPP

#include "vector.hpp"

namespace manager {
	enum class MovementType: int {
		LINEAR,
		ROTATIONAL,
	};
	
	struct LinearRotation {
		Vector destination;
		double speed;
	};

	struct MovementManagerRequest {
		
	};
}

#endif // MOVEMENT_MANAGER_REQUEST_HPP