#ifndef	MOVEMENT_VENDOR_API_REQUEST_HPP
#define	MOVEMENT_VENDOR_API_REQUEST_HPP

#include "movement_manager_request.hpp"
#include "vendor_api_request.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	enum class MovementManagerId: int {
		MOVEMENT_MANAGER,
	};

	class MovementVendorApiRequest: public ApiRequest<MovementManagerId, manager::MovementManagerRequest> {
	public:
		MovementVendorApiRequest(const Instance<manager::MovementManagerRequest>& manager_request);
		MovementVendorApiRequest(const MovementVendorApiRequest& other) = default;
		MovementVendorApiRequest& operator=(const MovementVendorApiRequest&) = default;

		MovementManagerId manager_id() const override;
		Instance<manager::MovementManagerRequest> payload() const override;
	private:
		Instance<manager::MovementManagerRequest> m_movement_request;
	};

	inline MovementVendorApiRequest::MovementVendorApiRequest(const Instance<manager::MovementManagerRequest>& manager_request): m_movement_request(manager_request) {}
	
	inline MovementManagerId MovementVendorApiRequest::manager_id() const {
		return MovementManagerId::MOVEMENT_MANAGER;
	}

	inline Instance<manager::MovementManagerRequest> MovementVendorApiRequest::payload() const {
		return m_movement_request;
	}
}

#endif // MOVEMENT_VENDOR_API_REQUEST_HPP