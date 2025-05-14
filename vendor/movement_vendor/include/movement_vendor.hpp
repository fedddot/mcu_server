#ifndef	MOVEMENT_VENDOR_HPP
#define	MOVEMENT_VENDOR_HPP

#include <stdexcept>

#include "vendor.hpp"

namespace vendor {
	template <typename AxisControllerConfig>
	class MovementVendor: public Vendor {
	public:
		MovementVendor();
		MovementVendor(const MovementVendor& other) = default;
		MovementVendor& operator=(const MovementVendor&) = delete;

		Instance<ApiResponse> run_api_request(const ApiRequest& request) override;
	private:
	};

	template <typename AxisControllerConfig>
	inline MovementVendor<AxisControllerConfig>::MovementVendor() {
		throw std::invalid_argument("NOT IMPLEMENTED");
	}
	
	template <typename AxisControllerConfig>
	inline Instance<ApiResponse> MovementVendor<AxisControllerConfig>::run_api_request(const ApiRequest& request) {
		throw std::invalid_argument("NOT IMPLEMENTED");
	}
}

#endif // MOVEMENT_VENDOR_HPP