#ifndef THERMOSTAT_VENDOR_HPP
#define THERMOSTAT_VENDOR_HPP

#include "thermostat_api_request.hpp"
#include "thermostat_api_response.hpp"
#include "thermostat_manager.hpp"
#include "vendor.hpp"
#include "vendor_instance.hpp"

namespace vendor {
    class ThermostatVendor: public Vendor<ThermostatVendorApiRequest, ThermostatVendorApiResponse> {
    public:
		using ThermostatManagerInstance = vendor::Instance<manager::ThermostatManager>;

		ThermostatVendor(
			const ThermostatManagerInstance& thermostat_manager
		);
		ThermostatVendor(const ThermostatVendor& other) = default;
		ThermostatVendor& operator=(const ThermostatVendor&) = delete;

		ThermostatVendorApiResponse run_api_request(const ThermostatVendorApiRequest& request) override;
	private:
		ThermostatManagerInstance m_thermostat_manager;
		
		ThermostatVendorApiResponse run_start_request(const ThermostatVendorApiRequest& request);
		ThermostatVendorApiResponse run_stop_request(const ThermostatVendorApiRequest& request);
    };
}

#endif // THERMOSTAT_VENDOR_HPP
