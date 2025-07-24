#ifndef THERMOSTAT_SERVICE_HPP
#define THERMOSTAT_SERVICE_HPP

#include <optional>

#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_manager_controller.hpp"
#include "service.hpp"

namespace service {
    class ThermostatService: public Service<ThermostatServiceApiRequest, ThermostatServiceApiResponse> {
    public:
		ThermostatService(
			manager::ThermostatController *controller_ptr
		): m_thermostat_manager(controller_ptr) {
            
        }
		ThermostatService(const ThermostatService& other) = default;
		ThermostatService& operator=(const ThermostatService&) = delete;

		ThermostatServiceApiResponse run_api_request(const ThermostatServiceApiRequest& request) override {
            switch (request.type()) {
            case ThermostatServiceApiRequest::RequestType::START:
                return run_start_request(request);
            case ThermostatServiceApiRequest::RequestType::STOP:
                return run_stop_request(request);
            case ThermostatServiceApiRequest::RequestType::GET_TEMP:
                return run_get_temp_request(request);
            default:
                return ThermostatServiceApiResponse(
                    ThermostatServiceApiResponse::Result::FAILURE,
                    "unknown request type"
                );
            }
        }
	private:
		manager::ThermostatManager m_thermostat_manager;
        ThermostatController *m_controller_ptr;
		std::optional<Instance<ThermostatController::TaskGuard>> m_regulation_task_guard;
		
		ThermostatServiceApiResponse run_start_request(const ThermostatServiceApiRequest& request) {
            if (!request.temperature() || !request.time_resolution_ms()) {
                return ThermostatServiceApiResponse(
                    ThermostatServiceApiResponse::Result::FAILURE,
                    "invalid start request: temperature or time resolution is missing"
                );
            }
            m_thermostat_manager.start(
                *request.temperature(),
                *request.time_resolution_ms()
            );
            return ThermostatServiceApiResponse(
                ThermostatServiceApiResponse::Result::SUCCESS,
                std::nullopt
            );
        }
		ThermostatServiceApiResponse run_stop_request(const ThermostatServiceApiRequest& request) {
            m_thermostat_manager.stop();
            return ThermostatServiceApiResponse(
                ThermostatServiceApiResponse::Result::SUCCESS,
                std::nullopt
            );
        }
        ThermostatServiceApiResponse run_get_temp_request(const ThermostatServiceApiRequest& request) {
            return ThermostatServiceApiResponse(
                ThermostatServiceApiResponse::Result::SUCCESS,
                std::nullopt,
                m_thermostat_manager.get_current_temperature()
            );
        }
        		void start(const double temp, const std::size_t time_resolution_ms) {
			if (m_regulation_task_guard) {
				m_regulation_task_guard->get().unschedule();
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
		}
		void stop() {
			if (m_regulation_task_guard) {
				m_regulation_task_guard->get().unschedule();
				m_regulation_task_guard = std::nullopt;
			}
			m_controller_ptr->set_relay_state(false);
		}
		double get_current_temperature() const {
			return m_controller_ptr->read_temperature();
		}
    };
}

#endif // THERMOSTAT_SERVICE_HPP
