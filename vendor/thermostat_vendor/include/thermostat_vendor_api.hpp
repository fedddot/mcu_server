#ifndef THERMOSTAT_VENDOR_API_HPP
#define THERMOSTAT_VENDOR_API_HPP

namespace vendor {
class ThermostatVendorApi {
public:
    virtual ~ThermostatVendorApi() noexcept = default;
    // Define vendor-specific API methods here
};
}

#endif // THERMOSTAT_VENDOR_API_HPP
