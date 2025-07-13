#ifndef	THERMOSTAT_VENDOR_API_RESPONSE_HPP
#define	THERMOSTAT_VENDOR_API_RESPONSE_HPP

#include <optional>
#include <string>

namespace vendor {
	class ThermostatVendorApiResponse {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE,
		};
		ThermostatVendorApiResponse(
			const Result& result = Result::FAILURE,
			const std::optional<std::string>& message = std::nullopt,
			const std::optional<double>& temperature = std::nullopt
		): m_result(result), m_message(message), m_temperature(temperature) {

		}
		ThermostatVendorApiResponse(const ThermostatVendorApiResponse& other) = default;
		ThermostatVendorApiResponse& operator=(const ThermostatVendorApiResponse&) = default;
		virtual ~ThermostatVendorApiResponse() noexcept = default;

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

#endif // THERMOSTAT_VENDOR_API_RESPONSE_HPP