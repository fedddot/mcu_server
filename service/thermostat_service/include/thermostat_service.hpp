#ifndef THERMOSTAT_SERVICE_HPP
#define THERMOSTAT_SERVICE_HPP

#include <optional>
#include <stdexcept>

#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_controller.hpp"
#include "service.hpp"

namespace service {
    class ThermostatService: public Service<ThermostatApiRequest, ThermostatServiceApiResponse> {
    public:
		ThermostatService(ThermostatController *controller_ptr): m_controller_ptr(controller_ptr), m_regulation_task_guard(std::nullopt) {
            if (!m_controller_ptr) {
                throw std::runtime_error("invalid controller ptr received");
            }
        }
		ThermostatService(const ThermostatService& other) = default;
		ThermostatService& operator=(const ThermostatService&) = delete;

		ThermostatServiceApiResponse run_api_request(const ThermostatApiRequest& request) override {
            switch (request.type()) {
            case ThermostatApiRequest::RequestType::START:
                return run_start_request(request);
            case ThermostatApiRequest::RequestType::STOP:
                return run_stop_request(request);
            case ThermostatApiRequest::RequestType::GET_TEMP:
                return run_get_temp_request(request);
            default:
                return ThermostatServiceApiResponse(
                    ThermostatServiceApiResponse::Result::FAILURE,
                    "unknown request type"
                );
            }
        }
	private:
        ThermostatController *m_controller_ptr;
		std::optional<ThermostatController::TaskGuard *> m_regulation_task_guard;

        ThermostatServiceApiResponse run_start_request(const ThermostatApiRequest& request) {
            if (!request.temperature().has_value() || !request.time_resolution_ms().has_value()) {
                return ThermostatServiceApiResponse(
                    ThermostatServiceApiResponse::Result::FAILURE,
                    "invalid start request: temperature or time resolution is missing"
                );
            }
            const auto temp = request.temperature().value();
			const auto time_resolution_ms = request.time_resolution_ms().value();
            if (m_regulation_task_guard) {
				m_regulation_task_guard.value()->unschedule();
				m_regulation_task_guard = std::nullopt;
			}
			m_regulation_task_guard = m_controller_ptr->schedule_task(
				[this, temp]() {
					if (m_controller_ptr->read_temperature() < temp) {
						m_controller_ptr->set_relay_state(true);
					} else {
						m_controller_ptr->set_relay_state(false);
					}
				},
				time_resolution_ms
			);
            return ThermostatServiceApiResponse(
                ThermostatServiceApiResponse::Result::SUCCESS,
                std::nullopt
            );
        }
		ThermostatServiceApiResponse run_stop_request(const ThermostatApiRequest& request) {
			if (m_regulation_task_guard.has_value()) {
				m_regulation_task_guard.value()->unschedule();
                delete m_regulation_task_guard.value();
				m_regulation_task_guard = std::nullopt;
			}
			m_controller_ptr->set_relay_state(false);
            return ThermostatServiceApiResponse(
                ThermostatServiceApiResponse::Result::SUCCESS,
                std::nullopt
            );
        }
        ThermostatServiceApiResponse run_get_temp_request(const ThermostatApiRequest& request) {
            return ThermostatServiceApiResponse(
                ThermostatServiceApiResponse::Result::SUCCESS,
                std::nullopt,
                m_controller_ptr->read_temperature()
            );
        }
    };
}

#endif // THERMOSTAT_SERVICE_HPP
