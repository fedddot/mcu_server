#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include "data.hpp"
#include "integer.hpp"
#include "inventory_manager.hpp"
#include "object.hpp"
#include "resource.hpp"
#include "stepper_motor.hpp"

namespace manager {
	
	class StepperMotorManager: public InventoryManager<StepperMotor> {
	public:
		StepperMotorManager(const Creator& creator);
		StepperMotorManager(const StepperMotorManager& other) = default;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;
		
		server::Resource *clone() const override;
	private:
		static StepperMotor *duplicate(const StepperMotor& instance);
		static server::Object read_motor(const StepperMotor& gpio);
		static void write_motor(StepperMotor *motor_ptr, const server::Data& data);
	};

	inline StepperMotorManager::StepperMotorManager(const Creator& creator): InventoryManager<StepperMotor>(creator, Duplicator(StepperMotorManager::duplicate), Reader(StepperMotorManager::read_motor), Writer(StepperMotorManager::write_motor)) {

	}

	inline server::Resource *StepperMotorManager::clone() const {
		return new StepperMotorManager(*this);
	}

	inline StepperMotor *StepperMotorManager::duplicate(const StepperMotor& instance) {
		return instance.clone();
	}
	
	inline server::Object StepperMotorManager::read_motor(const StepperMotor& gpio) {
		using namespace server;
		return Object();
	}

	inline void StepperMotorManager::write_motor(StepperMotor *motor_ptr, const server::Data& data) {
		using namespace server;
		const auto direction = static_cast<StepperMotor::Direction>(Data::cast<Integer>(Data::cast<Object>(data).access("dir")).get());
		const auto steps_num = static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("steps_num")).get());
		const auto step_duration_ms = static_cast<unsigned int>(Data::cast<Integer>(Data::cast<Object>(data).access("step_duration_ms")).get());
		motor_ptr->steps(direction, steps_num, step_duration_ms);
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP