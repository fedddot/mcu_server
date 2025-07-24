#ifndef	MOVEMENT_VENDOR_API_RESPONSE_HPP
#define	MOVEMENT_VENDOR_API_RESPONSE_HPP

#include <optional>
#include <string>

namespace vendor {
	class MovementVendorApiResponse {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE,
		};
		MovementVendorApiResponse(const Result& result = Result::FAILURE, const std::optional<std::string>& message = std::nullopt): m_result(result), m_message(message) {

		}
		MovementVendorApiResponse(const MovementVendorApiResponse& other) = default;
		MovementVendorApiResponse& operator=(const MovementVendorApiResponse&) = default;
		virtual ~MovementVendorApiResponse() noexcept = default;

		Result result() const {
			return m_result;
		}

		std::optional<std::string> message() const {
			return m_message;
		}
	private:
		Result m_result;
		std::optional<std::string> m_message;
	};
}

#endif // MOVEMENT_VENDOR_API_RESPONSE_HPP