#ifndef TIMER_SCHEDULER_HPP
#define TIMER_SCHEDULER_HPP

#include <cstddef>
#include <functional>

#include "manager_instance.hpp"

namespace manager {
    class TimerScheduler {
    public:
        using Task = std::function<void()>;
        class TaskGuard {
        public:
            virtual ~TaskGuard() noexcept = default;
            virtual void unschedule() = 0;
        };
        virtual ~TimerScheduler() noexcept = default;
        virtual Instance<TaskGuard> schedule_task(const Task& task, const std::size_t period_ms) = 0;
    };
}

#endif // TIMER_SCHEDULER_HPP
