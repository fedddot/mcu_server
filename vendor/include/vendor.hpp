#ifndef	VENDOR_HPP
#define	VENDOR_HPP

#include "vendor_api_request.hpp"
#include "vendor_api_response.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	class Vendor {
	public:
		virtual ~Vendor() noexcept = default;
		virtual Instance<ApiResponse> run_api_request(const ApiRequest& request) = 0;
	};
}

#endif // VENDOR_HPP