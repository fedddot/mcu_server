#ifndef	THERMOSTAT_MANAGER_HPP
#define	THERMOSTAT_MANAGER_HPP

#include <cstddef>
#include <optional>
#include <stdexcept>

#include "manager.hpp"
#include "manager_instance.hpp"
#include "thermostat_manager_controller.hpp"

namespace manager {
	class ThermostatManager: public Manager {
	public:
		ThermostatManager(ThermostatManagerController *controller_ptr): m_controller_ptr(controller_ptr), m_regulation_task_guard(std::nullopt) {
			if (!m_controller_ptr) {
				throw std::runtime_error("invalid controller_ptr received");
			}
			m_controller_ptr->set_relay_state(false);
		}
		ThermostatManager(const ThermostatManager& other) = default;
		ThermostatManager& operator=(const ThermostatManager&) = delete;
		~ThermostatManager() noexcept override {
			stop();
		}
		
		void start(const double temp, const std::size_t time_resolution_ms) {
			if (m_regulation_task_guard) {
				m_regulation_task_guard->get().unschedule();
				m_regulation_task_guard = std::nullopt;
			}
			m_regulation_task_guard = m_controller_ptr->schedule_task(
				[this, temp]() {
					if (m_controller_ptr->read_temperature() < temp) {
						m_controller_ptr->set_relay_state(true);
					} else {
						m_controller_ptr->set_relay_state(false);
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
			m_controller_ptr->set_relay_state(false);
		}
		double get_current_temperature() const {
			return m_controller_ptr->read_temperature();
		}
	private:
		ThermostatManagerController *m_controller_ptr;
		std::optional<Instance<ThermostatManagerController::TaskGuard>> m_regulation_task_guard;
	};
}

#endif // THERMOSTAT_MANAGER_HPP