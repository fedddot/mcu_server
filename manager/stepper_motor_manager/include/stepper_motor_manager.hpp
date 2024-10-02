#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include "data.hpp"
#include "gpi.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory_manager.hpp"

#include "object.hpp"
#include "resource.hpp"
#include <stdexcept>

namespace manager {
	
	class StepperMotorManager: public InventoryManager<StepperMotor> {
	public:
		StepperMotorManager(const Creator& creator);
		StepperMotorManager(const StepperMotorManager& other) = default;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;
		
		server::Resource *clone() const override;
	private:
		static StepperMotor *duplicate(const StepperMotor& instance);
		static server::Object read_gpio(const StepperMotor& gpio);
		static void write_gpio(StepperMotor *gpio_ptr, const server::Data& data);
		static typename StepperMotor::State read_gpio_state(const StepperMotor& gpio);
	};

	inline StepperMotorManager::StepperMotorManager(const Creator& creator): InventoryManager<StepperMotor>(creator, Duplicator(StepperMotorManager::duplicate), Reader(StepperMotorManager::read_gpio), Writer(StepperMotorManager::write_gpio)) {

	}

	inline server::Resource *StepperMotorManager::clone() const {
		return new StepperMotorManager(*this);
	}

	inline StepperMotor *StepperMotorManager::duplicate(const StepperMotor& instance) {
		return instance.clone();
	}
	
	inline server::Object StepperMotorManager::read_gpio(const StepperMotor& gpio) {
		using namespace server;
		Object gpio_data;
		gpio_data.add("dir", Integer(static_cast<int>(gpio.direction())));
		gpio_data.add("state", Integer(static_cast<int>(read_gpio_state(gpio))));
		return gpio_data;
	}

	inline void StepperMotorManager::write_gpio(StepperMotor *gpio_ptr, const server::Data& data) {
		using namespace server;
		const auto state = static_cast<StepperMotor::State>(Data::cast<Integer>(Data::cast<Object>(data).access("state")).get());
		StepperMotor::cast<Gpo>(*gpio_ptr).set_state(state);
	}

	inline typename StepperMotor::State StepperMotorManager::read_gpio_state(const StepperMotor& gpio) {
		switch (gpio.direction()) {
		case StepperMotor::Direction::IN:
			return StepperMotor::cast<Gpi>(gpio).state();
		case StepperMotor::Direction::OUT:
			return StepperMotor::cast<Gpo>(gpio).state();
		default:
			throw std::invalid_argument("unsupported StepperMotor type");
		}
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP