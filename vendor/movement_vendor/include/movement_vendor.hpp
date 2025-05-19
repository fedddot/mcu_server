#ifndef	MOVEMENT_VENDOR_HPP
#define	MOVEMENT_VENDOR_HPP

#include <stdexcept>

#include "axes_controller_config_request.hpp"
#include "movement_manager.hpp"
#include "movement_vendor_api_request.hpp"
#include "movement_vendor_api_response.hpp"
#include "vendor.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	template <typename AxesConfig>
	class MovementVendor: public Vendor<MovementVendorApiRequest, MovementVendorApiResponse> {
	public:
		using MovementManagerInstance = vendor::Instance<manager::MovementManager<AxesConfig>>;

		MovementVendor(
			const MovementManagerInstance& movement_manager
		);
		MovementVendor(const MovementVendor& other) = default;
		MovementVendor& operator=(const MovementVendor&) = delete;

		MovementVendorApiResponse run_api_request(const MovementVendorApiRequest& request) override;
	private:
		MovementManagerInstance m_movement_manager;

		
		MovementVendorApiResponse run_cfg_request(const MovementVendorApiRequest& request);

		template <typename T>
		static const T& cast_request(const MovementVendorApiRequest& request);
	};

	template <typename AxesConfig>
	inline MovementVendor<AxesConfig>::MovementVendor(
		const MovementManagerInstance& movement_manager
	): m_movement_manager(movement_manager) {}
	
	template <typename AxesConfig>
	inline MovementVendorApiResponse MovementVendor<AxesConfig>::run_api_request(const MovementVendorApiRequest& request) {
		switch (request.type()) {
		case MovementVendorApiRequest::RequestType::CONFIG:
			return run_cfg_request(request);
		default:
			throw std::invalid_argument("unsupported api request received");
		}
	}

	template <typename AxesConfig>
	inline MovementVendorApiResponse MovementVendor<AxesConfig>::run_cfg_request(const MovementVendorApiRequest& request) {
		try {
			auto casted_request = cast_request<AxesControllerConfigApiRequest<AxesConfig>>(request);
			m_movement_manager.get().init(casted_request.axes_cfg());
			return MovementVendorApiResponse(MovementVendorApiResponse::Result::SUCCESS);
		} catch (...) {
			return MovementVendorApiResponse(MovementVendorApiResponse::Result::FAILURE);
		}
	}
}

#endif // MOVEMENT_VENDOR_HPP