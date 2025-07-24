#ifndef	MOVEMENT_SERVICE_API_REQUEST_HPP
#define	MOVEMENT_SERVICE_API_REQUEST_HPP

namespace service {
	class MovementServiceApiRequest {
	public:
		enum class RequestType: int {
			CONFIG,
			LINEAR_MOVEMENT,
			ROTATIONAL_MOVEMENT,
		};
		virtual RequestType type() const = 0;
	};
}

#endif // MOVEMENT_SERVICE_API_REQUEST_HPP