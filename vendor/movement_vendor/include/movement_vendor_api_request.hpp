#ifndef	MOVEMENT_VENDOR_API_REQUEST_HPP
#define	MOVEMENT_VENDOR_API_REQUEST_HPP

#include "movement_manager_request.hpp"
#include "vendor_api_request.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	class MovementVendorApiRequest: public ApiRequest {
	public:
		MovementVendorApiRequest(manager::MovementManagerRequest *manager_request_ptr);
		MovementVendorApiRequest(const MovementVendorApiRequest& other) = default;
		MovementVendorApiRequest& operator=(const MovementVendorApiRequest&) = default;

		std::string route() const override;
		Instance<manager::MovementManagerRequest> manager_request() const;
	private:
		Instance<manager::MovementManagerRequest> m_movement_request;
	};

	inline MovementVendorApiRequest::MovementVendorApiRequest(manager::MovementManagerRequest *manager_request_ptr): m_movement_request(manager_request_ptr) {}
	
	inline std::string MovementVendorApiRequest::route() const {
		return "movement";
	}

	inline Instance<manager::MovementManagerRequest> MovementVendorApiRequest::manager_request() const {
		return m_movement_request;
	}
}

#endif // MOVEMENT_VENDOR_API_REQUEST_HPP