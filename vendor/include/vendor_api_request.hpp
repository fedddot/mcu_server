#ifndef	VENDOR_API_REQUEST_HPP
#define	VENDOR_API_REQUEST_HPP

#include "vendor_instance.hpp"

namespace vendor {
	template <typename ManagerId, typename Payload>
	class ApiRequest {
	public:
		virtual ~ApiRequest() noexcept = default;
		virtual ManagerId manager_id() const = 0;
		virtual Instance<Payload> payload() const = 0;
	};
}

#endif // VENDOR_API_REQUEST_HPP