#ifndef	MOVEMENT_VENDOR_API_RESPONSE_HPP
#define	MOVEMENT_VENDOR_API_RESPONSE_HPP

#include "movement_manager_response.hpp"
#include "vendor_api_response.hpp"

namespace vendor {
	class MovementVendorApiResponse: public ApiResponse {
	public:
		MovementVendorApiResponse(const manager::MovementManagerResponse& manager_response);
		MovementVendorApiResponse(const MovementVendorApiResponse& other) = default;
		MovementVendorApiResponse& operator=(const MovementVendorApiResponse&) = default;

		Code code() const override;
	private:
		Code m_code;

		static Code manager_response_to_code(const manager::MovementManagerResponse& manager_response);
	};

	inline MovementVendorApiResponse::MovementVendorApiResponse(const manager::MovementManagerResponse& manager_response): m_code(manager_response_to_code(manager_response)) {}
	
	inline ApiResponse::Code MovementVendorApiResponse::code() const {
		return m_code;
	}

	inline ApiResponse::Code MovementVendorApiResponse::manager_response_to_code(const manager::MovementManagerResponse& manager_response) {
		using ManagerCode = manager::MovementManagerResponse::ResultCode;
		switch (manager_response.code) {
		case ManagerCode::OK:
			return Code::OK;
		default:
			return Code::FAILURE;
		}
	}
}

#endif // MOVEMENT_VENDOR_API_RESPONSE_HPP