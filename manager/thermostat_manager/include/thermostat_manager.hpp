#ifndef	THERMOSTAT_MANAGER_HPP
#define	THERMOSTAT_MANAGER_HPP

#include <cstddef>
#include <stdexcept>

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
		): m_relay_controller(relay_controller), m_temp_sensor_controller(temp_sensor_controller), m_timer_scheduler(timer_scheduler), m_is_running(false), m_time_resolution_ms(10) {
			m_relay_controller.get().set_relay_state(false);
		}
		ThermostatManager(const ThermostatManager& other) = default;
		ThermostatManager& operator=(const ThermostatManager&) = delete;
		
		void start(const double temp) {
			if (m_is_running) {
				throw std::runtime_error("ThermostatManager is already running");
			}
			throw std::runtime_error("not implemented");
		}
		void stop() {
			throw std::runtime_error("not implemented");
		}
		double get_current_temperature() const {
			throw std::runtime_error("not implemented");
		}
	private:
		Instance<RelayController> m_relay_controller;
		Instance<TemperatureSensorController> m_temp_sensor_controller;
		Instance<TimerScheduler> m_timer_scheduler;
		bool m_is_running;
		std::size_t m_time_resolution_ms;
	};
}

#endif // THERMOSTAT_MANAGER_HPP