#ifndef	MOVEMENT_MANAGER_RESPONSE_HPP
#define	MOVEMENT_MANAGER_RESPONSE_HPP

#include <optional>
#include <string>

namespace manager {
	struct MovementManagerResponse {
		enum class ResultCode: int {
			OK,
			BAD_REQUEST,
			EXCEPTION,
		};
		
		ResultCode code;
		std::optional<std::string> message;
	};
}

#endif // MOVEMENT_MANAGER_RESPONSE_HPP