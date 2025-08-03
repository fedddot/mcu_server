#ifndef THERMOSTAT_SERVICE_API_REQUEST_HPP
#define THERMOSTAT_SERVICE_API_REQUEST_HPP

#include <cstddef>
#include <optional>

namespace service {
	class ThermostatApiRequest {
	public:
		enum class RequestType: int {
			START,
			STOP,
			GET_TEMP,
		};
		ThermostatApiRequest(
			const RequestType& type = RequestType::STOP,
			const std::optional<double>& temp = std::nullopt,
			const std::optional<std::size_t>& time_resolution_ms = std::nullopt
		): m_type(type), m_temp(temp), m_time_resolution_ms(time_resolution_ms) {

		}
		ThermostatApiRequest(const ThermostatApiRequest& other) = default;
		ThermostatApiRequest& operator=(const ThermostatApiRequest&) = default;
		virtual ~ThermostatApiRequest() noexcept = default;

		RequestType type() const {
			return m_type;
		}

		std::optional<double> temperature() const {
			return m_temp;
		}

		std::optional<std::size_t> time_resolution_ms() const {
			return m_time_resolution_ms;
		}
	private:
		RequestType m_type;
		std::optional<double> m_temp;
		std::optional<std::size_t> m_time_resolution_ms;
	};
}

#endif // THERMOSTAT_SERVICE_API_REQUEST_HPP