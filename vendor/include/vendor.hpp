#ifndef	VENDOR_HPP
#define	VENDOR_HPP

#include "vendor_api_request.hpp"
#include "vendor_api_response.hpp"
#include "vendor_instance.hpp"

namespace vendor {
	template <typename ManagerId, typename Payload>
	class Vendor {
	public:
		virtual ~Vendor() noexcept = default;
		virtual Instance<ApiResponse> run_api_request(const ApiRequest<ManagerId, Payload>& request) = 0;
	};
}

#endif // VENDOR_HPP