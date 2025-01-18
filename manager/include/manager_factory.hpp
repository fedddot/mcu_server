#ifndef	MANAGER_FACTORY_HPP
#define	MANAGER_FACTORY_HPP

#include <stdexcept>

#include "manager.hpp"
#include "manager_config.hpp"
#include "stepper_motor_manager.hpp"
#include "stepper_motor_manager_config.hpp"

namespace manager {
	template <typename Request, typename Response, typename StepperCreateConfig>
	class ManagerFactory {
	public:
		ManagerFactory() = default;
		virtual ~ManagerFactory() noexcept = default;
		Manager<Request, Response> *operator()(const ManagerConfig& config) const;
	private:
		template <typename Config>
		static const Config& cast_config(const ManagerConfig& config);
	};

	template <typename Request, typename Response, typename StepperCreateConfig>
	inline Manager<Request, Response> *ManagerFactory<Request, Response, StepperCreateConfig>::operator()(const ManagerConfig& config) const {
		if ("manager.stepper_motor" == config.type()) {
			return new StepperMotorManager<StepperCreateConfig>(cast_config<StepperMotorManagerConfig<StepperCreateConfig>>(config));
		}
		throw std::invalid_argument("unsupported manager type");
	}

	template <typename Request, typename Response, typename StepperCreateConfig>
	template <typename Config>
	const Config& ManagerFactory<Request, Response, StepperCreateConfig>::cast_config(const ManagerConfig& config) {
		try {
			return dynamic_cast<const Config&>(config);
		} catch (...) {
			throw std::runtime_error("failed to cast manager config into target type: " + config.type());
		}
	}
}

#endif // MANAGER_FACTORY_HPP