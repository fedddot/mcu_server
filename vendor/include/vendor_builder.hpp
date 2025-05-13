#ifndef	VENDOR_BUILDER_HPP
#define	VENDOR_BUILDER_HPP

namespace vendor {
	template <typename ApiRequest, typename ApiResponse>
	class VendorBuilder {
	public:
		virtual ~VendorBuilder() noexcept = default;
		virtual ApiResponse run_api_request(const ApiRequest& request) = 0;
	};
}

#endif // VENDOR_BUILDER_HPP