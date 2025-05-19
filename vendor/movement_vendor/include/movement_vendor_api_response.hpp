#ifndef	MOVEMENT_VENDOR_API_RESPONSE_HPP
#define	MOVEMENT_VENDOR_API_RESPONSE_HPP

namespace vendor {
	class MovementVendorApiResponse {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE,
		};
		MovementVendorApiResponse(const Result& result): m_result(result) {

		}
		MovementVendorApiResponse(const MovementVendorApiResponse& other) = default;
		MovementVendorApiResponse& operator=(const MovementVendorApiResponse&) = default;
		virtual ~MovementVendorApiResponse() noexcept = default;

		Result result() const {
			return m_result;
		}
	private:
		Result m_result;
	};
}

#endif // MOVEMENT_VENDOR_API_RESPONSE_HPP