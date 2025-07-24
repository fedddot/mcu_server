#ifndef THERMOSTAT_VENDOR_HPP
#define THERMOSTAT_VENDOR_HPP

#include <optional>

#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_manager.hpp"
#include "thermostat_manager_controller.hpp"
#include "vendor.hpp"

namespace vendor {
    class ThermostatVendor: public Vendor<ThermostatVendorApiRequest, ThermostatVendorApiResponse> {
    public:
		ThermostatVendor(
			manager::ThermostatManagerController *controller_ptr
		): m_thermostat_manager(controller_ptr) {
            
        }
		ThermostatVendor(const ThermostatVendor& other) = default;
		ThermostatVendor& operator=(const ThermostatVendor&) = delete;

		ThermostatVendorApiResponse run_api_request(const ThermostatVendorApiRequest& request) override {
            switch (request.type()) {
            case ThermostatVendorApiRequest::RequestType::START:
                return run_start_request(request);
            case ThermostatVendorApiRequest::RequestType::STOP:
                return run_stop_request(request);
            case ThermostatVendorApiRequest::RequestType::GET_TEMP:
                return run_get_temp_request(request);
            default:
                return ThermostatVendorApiResponse(
                    ThermostatVendorApiResponse::Result::FAILURE,
                    "unknown request type"
                );
            }
        }
	private:
		manager::ThermostatManager m_thermostat_manager;
		
		ThermostatVendorApiResponse run_start_request(const ThermostatVendorApiRequest& request) {
            if (!request.temperature() || !request.time_resolution_ms()) {
                return ThermostatVendorApiResponse(
                    ThermostatVendorApiResponse::Result::FAILURE,
                    "invalid start request: temperature or time resolution is missing"
                );
            }
            m_thermostat_manager.start(
                *request.temperature(),
                *request.time_resolution_ms()
            );
            return ThermostatVendorApiResponse(
                ThermostatVendorApiResponse::Result::SUCCESS,
                std::nullopt
            );
        }
		ThermostatVendorApiResponse run_stop_request(const ThermostatVendorApiRequest& request) {
            m_thermostat_manager.stop();
            return ThermostatVendorApiResponse(
                ThermostatVendorApiResponse::Result::SUCCESS,
                std::nullopt
            );
        }
        ThermostatVendorApiResponse run_get_temp_request(const ThermostatVendorApiRequest& request) {
            return ThermostatVendorApiResponse(
                ThermostatVendorApiResponse::Result::SUCCESS,
                std::nullopt,
                m_thermostat_manager.get_current_temperature()
            );
        }
    };
}

#endif // THERMOSTAT_VENDOR_HPP
