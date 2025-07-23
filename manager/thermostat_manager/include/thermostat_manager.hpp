#ifndef	THERMOSTAT_MANAGER_HPP
#define	THERMOSTAT_MANAGER_HPP

#include <cstddef>
#include <optional>

#include "manager.hpp"
#include "manager_instance_reference.hpp"
#include "relay_controller.hpp"
#include "temperature_sensor_controller.hpp"
#include "timer_scheduler.hpp"

namespace manager {
	class ThermostatManager: public Manager {
	public:
		ThermostatManager(
			const InstanceReference<RelayController>& relay_controller_ref,
			const InstanceReference<TemperatureSensorController>& temp_sensor_controller_ref,
			const InstanceReference<TimerScheduler>& timer_scheduler_ref
		):	m_relay_controller_ref(relay_controller_ref),
			m_temp_sensor_controller_ref(temp_sensor_controller_ref),
			m_timer_scheduler_ref(timer_scheduler_ref),
			m_regulation_task_guard(std::nullopt)
		{			
			m_relay_controller_ref.get().set_relay_state(false);
		}
		ThermostatManager(const ThermostatManager& other) = default;
		ThermostatManager& operator=(const ThermostatManager&) = delete;
		
		void start(const double temp, const std::size_t time_resolution_ms) {
			if (m_regulation_task_guard) {
				m_regulation_task_guard->get().unschedule();
				m_regulation_task_guard = std::nullopt;
			}
			m_regulation_task_guard = m_timer_scheduler_ref.get().schedule_task(
				[this, temp]() {
					if (m_temp_sensor_controller_ref.get().read_temperature() < temp) {
						m_relay_controller_ref.get().set_relay_state(true);
					} else {
						m_relay_controller_ref.get().set_relay_state(false);
					}
				},
				time_resolution_ms
			);
		}
		void stop() {
			if (m_regulation_task_guard) {
				m_regulation_task_guard->get().unschedule();
				m_regulation_task_guard = std::nullopt;
			}
			m_relay_controller_ref.get().set_relay_state(false);
		}
		double get_current_temperature() const {
			return m_temp_sensor_controller_ref.get().read_temperature();
		}
	private:
		InstanceReference<RelayController> m_relay_controller_ref;
		InstanceReference<TemperatureSensorController> m_temp_sensor_controller_ref;
		InstanceReference<TimerScheduler> m_timer_scheduler_ref;
		std::optional<Instance<TimerScheduler::TaskGuard>> m_regulation_task_guard;
	};
}

#endif // THERMOSTAT_MANAGER_HPP