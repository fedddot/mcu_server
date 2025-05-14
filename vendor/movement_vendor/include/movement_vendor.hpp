#ifndef	MOVEMENT_VENDOR_HPP
#define	MOVEMENT_VENDOR_HPP

#include "manager_instance.hpp"
#include "manager.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_vendor_api_request.hpp"
#include "movement_vendor_api_response.hpp"
#include "vendor.hpp"
#include "vendor_api_response.hpp"

namespace vendor {
	template <typename AxisControllerConfig>
	class MovementVendor: public Vendor<MovementManagerId, manager::MovementManagerRequest> {
	public:
		using MovementManagerInstance = manager::Instance<manager::Manager<manager::MovementManagerRequest, manager::MovementManagerResponse>>;

		MovementVendor(
			const MovementManagerInstance& movement_manager
		);
		MovementVendor(const MovementVendor& other) = default;
		MovementVendor& operator=(const MovementVendor&) = delete;

		Instance<ApiResponse> run_api_request(const ApiRequest<MovementManagerId, manager::MovementManagerRequest>& request) override;
	private:
		MovementManagerInstance m_movement_manager;
		static Instance<ApiResponse> cast_response(const manager::MovementManagerResponse& response);
	};

	template <typename AxisControllerConfig>
	inline MovementVendor<AxisControllerConfig>::MovementVendor(
		const MovementManagerInstance& movement_manager
	): m_movement_manager(movement_manager) {}
	
	template <typename AxisControllerConfig>
	inline Instance<ApiResponse> MovementVendor<AxisControllerConfig>::run_api_request(const ApiRequest<MovementManagerId, manager::MovementManagerRequest>& request) {
		const auto manager_response = m_movement_manager.get().run(request.payload().get());
		return Instance<ApiResponse>(new MovementVendorApiResponse(manager_response));
	}
}

#endif // MOVEMENT_VENDOR_HPP