#ifndef	MOTOR_DRIVE_SERVICE_API_RESPONSE_HPP
#define	MOTOR_DRIVE_SERVICE_API_RESPONSE_HPP

#include <functional>
#include <optional>

namespace service {
	template <typename Status>
	class MotorDriveServiceApiResponse {
	public:
		enum class Result: int {
			SUCCESS = 0,
			FAILURE = 1
		};
		MotorDriveServiceApiResponse(
			const Result& result,
			const std::optional<Status>& status
		): m_result(result), m_status(status) {}
		MotorDriveServiceApiResponse(const MotorDriveServiceApiResponse&) = default;
		MotorDriveServiceApiResponse& operator=(const MotorDriveServiceApiResponse&) = default;
		virtual ~MotorDriveServiceApiResponse() noexcept = default;
		const Result& result() const { return std::ref(m_result); }
		const std::optional<Status>& status() const { return std::ref(m_status); }
	private:
		Result m_result;
		std::optional<Status> m_status;
	};
}

#endif // MOTOR_DRIVE_SERVICE_API_RESPONSE_HPP