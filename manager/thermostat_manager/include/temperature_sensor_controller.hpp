#ifndef TEMPERATURE_SENSOR_CONTROLLER_HPP
#define TEMPERATURE_SENSOR_CONTROLLER_HPP

namespace manager {
    class TemperatureSensorController {
    public:
        virtual ~TemperatureSensorController() noexcept = default;
        virtual double read_temperature() const = 0;
    };
}

#endif // TEMPERATURE_SENSOR_CONTROLLER_HPP
