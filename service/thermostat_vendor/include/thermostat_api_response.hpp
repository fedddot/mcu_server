#ifndef	THERMOSTAT_SERVICE_API_RESPONSE_HPP
#define	THERMOSTAT_SERVICE_API_RESPONSE_HPP

#include <optional>
#include <string>

namespace service {
	class ThermostatServiceApiResponse {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE,
		};
		ThermostatServiceApiResponse(
			const Result& result = Result::FAILURE,
			const std::optional<std::string>& message = std::nullopt,
			const std::optional<double>& temperature = std::nullopt
		): m_result(result), m_message(message), m_temperature(temperature) {

		}
		ThermostatServiceApiResponse(const ThermostatServiceApiResponse& other) = default;
		ThermostatServiceApiResponse& operator=(const ThermostatServiceApiResponse&) = default;
		virtual ~ThermostatServiceApiResponse() noexcept = default;

		Result result() const {
			return m_result;
		}

		std::optional<std::string> message() const {
			return m_message;
		}

		std::optional<double> temperature() const {
			return m_temperature;
		}
	private:
		Result m_result;
		std::optional<std::string> m_message;
		std::optional<double> m_temperature;
	};
}

#endif // THERMOSTAT_SERVICE_API_RESPONSE_HPP