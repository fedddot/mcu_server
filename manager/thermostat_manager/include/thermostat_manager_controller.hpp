#ifndef THERMOSTAT_MANAGER_CONTROLLER_HPP
#define THERMOSTAT_MANAGER_CONTROLLER_HPP

#include <functional>

#include "manager_instance.hpp"

namespace manager {
    class TemperatureManagerController {
    public:
        using Task = std::function<void()>;
        class TaskGuard {
        public:
            virtual ~TaskGuard() noexcept = default;
            virtual void unschedule() = 0;
        };
        virtual ~TemperatureManagerController() noexcept = default;
        virtual double read_temperature() const = 0;
        virtual void set_relay_state(const bool state) = 0;
        virtual Instance<TaskGuard> schedule_task(const Task& task, const std::size_t period_ms) = 0;
    };
}

#endif // THERMOSTAT_MANAGER_CONTROLLER_HPP
