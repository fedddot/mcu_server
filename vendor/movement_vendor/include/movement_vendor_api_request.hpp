#ifndef	MOVEMENT_VENDOR_API_REQUEST_HPP
#define	MOVEMENT_VENDOR_API_REQUEST_HPP

namespace vendor {
	class MovementVendorApiRequest {
	public:
		enum class RequestType: int {
			CONFIG,
			LINEAR_MOVEMENT,
			ROTATIONAL_MOVEMENT,
		};
		virtual RequestType type() const = 0;
	};
}

#endif // MOVEMENT_VENDOR_API_REQUEST_HPP