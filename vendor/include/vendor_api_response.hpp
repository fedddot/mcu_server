#ifndef	VENDOR_API_RESPONSE_HPP
#define	VENDOR_API_RESPONSE_HPP

namespace vendor {
	class ApiResponse {
	public:
		enum class Code: int {
			OK,
			FAILURE,
		};
		virtual ~ApiResponse() noexcept = default;
		virtual Code code() const = 0;
	};
}

#endif // VENDOR_API_RESPONSE_HPP