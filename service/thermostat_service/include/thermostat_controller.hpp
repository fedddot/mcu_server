#ifndef THERMOSTAT_CONTROLLER_HPP
#define THERMOSTAT_CONTROLLER_HPP

#include <functional>

namespace service {
    class ThermostatController {
    public:
        using Task = std::function<void()>;
        class TaskGuard {
        public:
            virtual ~TaskGuard() noexcept = default;
            virtual void unschedule() = 0;
        };
        virtual ~ThermostatController() noexcept = default;
        virtual double read_temperature() const = 0;
        virtual void set_relay_state(const bool state) = 0;
        virtual TaskGuard *schedule_task(const Task& task, const std::size_t period_ms) = 0;
    };
}

#endif // THERMOSTAT_CONTROLLER_HPP
