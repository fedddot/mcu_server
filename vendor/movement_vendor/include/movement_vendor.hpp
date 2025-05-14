#ifndef	MOVEMENT_VENDOR_HPP
#define	MOVEMENT_VENDOR_HPP

#include <stdexcept>

#include "manager_instance.hpp"
#include "movement_manager.hpp"
#include "vendor.hpp"

namespace vendor {
	template <typename AxisControllerConfig>
	class MovementVendor: public Vendor {
	public:
		using MovementManagerInstance = manager::Instance<manager::MovementManager<AxisControllerConfig>>;
		MovementVendor(
			const MovementManagerInstance& movement_manager
		);
		MovementVendor(const MovementVendor& other) = default;
		MovementVendor& operator=(const MovementVendor&) = delete;

		Instance<ApiResponse> run_api_request(const ApiRequest& request) override;
	private:
		MovementManagerInstance m_movement_manager;
	};

	template <typename AxisControllerConfig>
	inline MovementVendor<AxisControllerConfig>::MovementVendor(
		const MovementManagerInstance& movement_manager
	): m_movement_manager(movement_manager) {}
	
	template <typename AxisControllerConfig>
	inline Instance<ApiResponse> MovementVendor<AxisControllerConfig>::run_api_request(const ApiRequest& request) {
		throw std::invalid_argument("NOT IMPLEMENTED");
	}
}

#endif // MOVEMENT_VENDOR_HPP