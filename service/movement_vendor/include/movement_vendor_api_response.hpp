#ifndef	MOVEMENT_SERVICE_API_RESPONSE_HPP
#define	MOVEMENT_SERVICE_API_RESPONSE_HPP

#include <optional>
#include <string>

namespace service {
	class MovementServiceApiResponse {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE,
		};
		MovementServiceApiResponse(const Result& result = Result::FAILURE, const std::optional<std::string>& message = std::nullopt): m_result(result), m_message(message) {

		}
		MovementServiceApiResponse(const MovementServiceApiResponse& other) = default;
		MovementServiceApiResponse& operator=(const MovementServiceApiResponse&) = default;
		virtual ~MovementServiceApiResponse() noexcept = default;

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

#endif // MOVEMENT_SERVICE_API_RESPONSE_HPP