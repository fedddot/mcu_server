#ifndef	VENDOR_API_REQUEST_HPP
#define	VENDOR_API_REQUEST_HPP

#include <string>

namespace vendor {
	class ApiRequest {
	public:
		virtual ~ApiRequest() noexcept = default;
		virtual std::string route() const = 0;
	};
}

#endif // VENDOR_API_REQUEST_HPP