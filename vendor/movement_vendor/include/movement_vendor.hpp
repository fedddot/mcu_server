#ifndef	MOVEMENT_VENDOR_HPP
#define	MOVEMENT_VENDOR_HPP

#include <stdexcept>

#include "manager.hpp"
#include "movement_vendor_api_request.hpp"
#include "movement_vendor_api_response.hpp"
#include "vendor.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	template <typename AxesConfig>
	class MovementVendor: public Vendor<MovementVendorApiRequest, MovementVendorApiResponse> {
	public:
		using MovementManagerInstance = vendor::Instance<manager::Manager>;

		MovementVendor(
			const MovementManagerInstance& movement_manager
		);
		MovementVendor(const MovementVendor& other) = default;
		MovementVendor& operator=(const MovementVendor&) = delete;

		MovementVendorApiResponse run_api_request(const MovementVendorApiRequest& request) override;
	private:
		MovementManagerInstance m_movement_manager;
	};

	template <typename AxesConfig>
	inline MovementVendor<AxesConfig>::MovementVendor(
		const MovementManagerInstance& movement_manager
	): m_movement_manager(movement_manager) {}
	
	template <typename AxesConfig>
	inline MovementVendorApiResponse MovementVendor<AxesConfig>::run_api_request(const MovementVendorApiRequest& request) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // MOVEMENT_VENDOR_HPP