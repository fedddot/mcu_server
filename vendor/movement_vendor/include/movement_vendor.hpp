#ifndef	MOVEMENT_VENDOR_HPP
#define	MOVEMENT_VENDOR_HPP

#include <stdexcept>

#include "manager_instance.hpp"
#include "manager.hpp"
#include "movement_manager_request.hpp"
#include "movement_manager_response.hpp"
#include "movement_vendor_api_request.hpp"
#include "vendor.hpp"
#include "vendor_api_response.hpp"

namespace vendor {
	template <typename AxisControllerConfig>
	class MovementVendor: public Vendor {
	public:
		using MovementManagerInstance = manager::Instance<manager::Manager<manager::MovementManagerRequest, manager::MovementManagerResponse>>;

		MovementVendor(
			const MovementManagerInstance& movement_manager
		);
		MovementVendor(const MovementVendor& other) = default;
		MovementVendor& operator=(const MovementVendor&) = delete;

		Instance<ApiResponse> run_api_request(const ApiRequest& request) override;
	private:
		MovementManagerInstance m_movement_manager;
		static Instance<manager::MovementManagerRequest> cast_request(const ApiRequest& request);
		static Instance<ApiResponse> cast_response(const manager::MovementManagerResponse& response);
	};

	template <typename AxisControllerConfig>
	inline MovementVendor<AxisControllerConfig>::MovementVendor(
		const MovementManagerInstance& movement_manager
	): m_movement_manager(movement_manager) {}
	
	template <typename AxisControllerConfig>
	inline Instance<ApiResponse> MovementVendor<AxisControllerConfig>::run_api_request(const ApiRequest& request) {
		const auto movement_api_request = cast_request(request);
		const auto manager_response = m_movement_manager.get().run(movement_api_request.get());
		const auto api_response = cast_response(manager_response);
		return api_response;
	}

	template <typename AxisControllerConfig>
	inline Instance<manager::MovementManagerRequest> MovementVendor<AxisControllerConfig>::cast_request(const ApiRequest& request) {
		try {
			const auto& movement_request = dynamic_cast<const MovementVendorApiRequest&>(request);
			return movement_request.manager_request();
		} catch (...) {
			throw std::runtime_error("failed to cast api request to movement api request");
		}
	}

	template <typename AxisControllerConfig>
	inline Instance<ApiResponse> MovementVendor<AxisControllerConfig>::cast_response(const manager::MovementManagerResponse& response) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // MOVEMENT_VENDOR_HPP