#ifndef	THERMOSTAT_MANAGER_HPP
#define	THERMOSTAT_MANAGER_HPP

#include "manager.hpp"
#include "manager_instance.hpp"
#include <cstddef>

namespace manager {
	class RelayController {
	public:
		virtual ~RelayController() noexcept = default;
		virtual void set_relay_state(const bool state) = 0;
	};
	class TemperatureSensorController {
	public:
		virtual ~TemperatureSensorController() noexcept = default;
		virtual double read_temperature() const = 0;
	};
	class ThermostatManager: public Manager {
	public:
		ThermostatManager(
			const Instance<RelayController>& relay_controller,
			const Instance<TemperatureSensorController>& temp_sensor_controller
		): m_relay_controller(relay_controller), m_temp_sensor_controller(temp_sensor_controller), m_is_running(false), m_time_resolution_ms(10) {
			m_relay_controller.get().set_relay_state(false);
		}
		ThermostatManager(const ThermostatManager& other) = default;
		ThermostatManager& operator=(const ThermostatManager&) = delete;
		
		void start(const double temp) {
			if (m_is_running) {
				throw std::runtime_error("ThermostatManager is already running");
		}
		void stop();
		double get_current_temperature() const;
	private:
		Instance<RelayController> m_relay_controller;
		Instance<TemperatureSensorController> m_temp_sensor_controller;
		bool m_is_running;
		std::size_t m_time_resolution_ms;
	};
}

#endif // THERMOSTAT_MANAGER_HPP