#ifndef	MOVEMENT_MANAGER_REQUEST_HPP
#define	MOVEMENT_MANAGER_REQUEST_HPP

#include <stdexcept>

namespace manager {
	class MovementManagerRequest {
	public:
		enum class RequestType: int {
			INIT,
			UNINIT,
			LINEAR_MOVEMENT,
			ROTATIONAL_MOVEMENT,
		};		
		virtual ~MovementManagerRequest() noexcept = default;
		virtual RequestType type() const = 0;
	};
}

#endif // MOVEMENT_MANAGER_REQUEST_HPP