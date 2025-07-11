#ifndef	THERMOSTAT_MANAGER_HPP
#define	THERMOSTAT_MANAGER_HPP

#include "manager.hpp"

namespace manager {
	class ThermostatManager: public Manager {
	public:
		ThermostatManager(
		);
		ThermostatManager(const ThermostatManager& other) = default;
		ThermostatManager& operator=(const ThermostatManager&) = delete;

	private:
	};
}

#endif // THERMOSTAT_MANAGER_HPP