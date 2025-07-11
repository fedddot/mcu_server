#ifndef	THERMOSTAT_MANAGER_HPP
#define	THERMOSTAT_MANAGER_HPP

#include <cstddef>
#include <optional>

#include "manager.hpp"
#include "manager_instance.hpp"
#include "relay_controller.hpp"
#include "temperature_sensor_controller.hpp"
#include "timer_scheduler.hpp"

namespace manager {
	class ThermostatManager: public Manager {
	public:
		ThermostatManager(
			const Instance<RelayController>& relay_controller,
			const Instance<TemperatureSensorController>& temp_sensor_controller,
			const Instance<TimerScheduler>& timer_scheduler
		): m_relay_controller(relay_controller), m_temp_sensor_controller(temp_sensor_controller), m_timer_scheduler(timer_scheduler), m_regulation_task_guard(std::nullopt), m_time_resolution_ms(10) {
			m_relay_controller.get().set_relay_state(false);
		}
		ThermostatManager(const ThermostatManager& other) = default;
		ThermostatManager& operator=(const ThermostatManager&) = delete;
		
		void start(const double temp) {
			if (m_regulation_task_guard) {
				m_regulation_task_guard->get().unschedule();
				m_regulation_task_guard = std::nullopt;
			}
			m_regulation_task_guard = m_timer_scheduler.get().schedule_task(
				[this, temp]() {
					if (m_temp_sensor_controller.get().read_temperature() < temp) {
						m_relay_controller.get().set_relay_state(true);
					} else {
						m_relay_controller.get().set_relay_state(false);
					}
				},
				m_time_resolution_ms
			);
		}
		void stop() {
			if (m_regulation_task_guard) {
				m_regulation_task_guard->get().unschedule();
				m_regulation_task_guard = std::nullopt;
			}
			m_relay_controller.get().set_relay_state(false);
		}
		double get_current_temperature() const {
			return m_temp_sensor_controller.get().read_temperature();
		}
	private:
		Instance<RelayController> m_relay_controller;
		Instance<TemperatureSensorController> m_temp_sensor_controller;
		Instance<TimerScheduler> m_timer_scheduler;
		std::optional<Instance<TimerScheduler::TaskGuard>> m_regulation_task_guard;
		std::size_t m_time_resolution_ms;
	};
}

#endif // THERMOSTAT_MANAGER_HPP