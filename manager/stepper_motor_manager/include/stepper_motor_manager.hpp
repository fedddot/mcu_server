#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include "data.hpp"
#include "stepper_motor.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_manager.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "string.hpp"

namespace manager {
	class StepperMotorManager: public InventoryManager<StepperMotor> {
	public:
		using StepperMotorCreator = typename InventoryManager<StepperMotor>::ItemCreator;
		StepperMotorManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const StepperMotorCreator& stepper_motor_creator);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;
	private:
		static server::Object read_stepper_motor(const StepperMotor& stepper_motor);
		static void write_stepper_motor(StepperMotor *stepper_motor, const server::Data& update_cfg);
	};
	
	inline StepperMotorManager::StepperMotorManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const StepperMotorCreator& stepper_motor_creator): InventoryManager<StepperMotor>(stepper_motor_inventory, stepper_motor_creator, read_stepper_motor, write_stepper_motor) {

	}

	inline server::Object StepperMotorManager::read_stepper_motor(const StepperMotor& stepper_motor) {
		using namespace server;
		Object stepper_motor_data;
		stepper_motor_data.add("enabled", Integer(static_cast<int>(stepper_motor.enabled())));
		return stepper_motor_data;
	}
	
	inline void StepperMotorManager::write_stepper_motor(StepperMotor *stepper_motor, const server::Data& update_cfg) {
		using namespace server;
		const auto& update_cfg_obj(Data::cast<Object>(update_cfg));
		const auto update_type(Data::cast<String>(update_cfg_obj.access("type")).get());
		if ("update_enable_status" == update_type) {
			const auto enable_status(Data::cast<Integer>(update_cfg_obj.access("enable")).get());
			if (!enable_status) {
				stepper_motor->enable();
				return;
			}
			stepper_motor->disable();
			return;
		} else if ("steps" == update_type) {
			const auto direction(static_cast<StepperMotor::Direction>(Data::cast<Integer>(update_cfg_obj.access("direction")).get()));
			auto steps_number(static_cast<unsigned int>(Data::cast<Integer>(update_cfg_obj.access("steps_number")).get()));
			while (steps_number) {
				stepper_motor->step(direction);
				--steps_number;
			}
		}
		throw ServerException(ResponseCode::BAD_REQUEST, "wrong update type: " + update_type);
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP