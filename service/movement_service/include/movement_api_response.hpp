#ifndef	MOVEMENT_API_RESPONSE_HPP
#define	MOVEMENT_API_RESPONSE_HPP

#include <optional>
#include <string>

namespace service {
	class MovementApiResponse {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE,
		};
		MovementApiResponse(const Result& result = Result::FAILURE, const std::optional<std::string>& message = std::nullopt): m_result(result), m_message(message) {

		}
		MovementApiResponse(const MovementApiResponse& other) = default;
		MovementApiResponse& operator=(const MovementApiResponse&) = default;
		virtual ~MovementApiResponse() noexcept = default;

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

#endif // MOVEMENT_API_RESPONSE_HPP