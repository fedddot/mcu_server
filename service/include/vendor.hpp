#ifndef	VENDOR_HPP
#define	VENDOR_HPP

namespace vendor {
	template <typename ApiRequest, typename ApiResponse>
	class Vendor {
	public:
		virtual ~Vendor() noexcept = default;
		virtual ApiResponse run_api_request(const ApiRequest& request) = 0;
	};
}

#endif // VENDOR_HPP