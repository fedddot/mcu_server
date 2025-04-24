#ifndef	MOVEMENT_MANAGER_RESPONSE_HPP
#define	MOVEMENT_MANAGER_RESPONSE_HPP

namespace manager {
	struct MovementManagerResponse {
		enum class ResultCode: int {
			OK,
			EXCEPTION
		};
		ResultCode code;
	};
}

#endif // MOVEMENT_MANAGER_RESPONSE_HPP