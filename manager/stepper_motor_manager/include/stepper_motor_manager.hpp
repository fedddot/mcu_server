#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <map>
#include <stdexcept>
#include <string>

#include "array.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "gpo_stepper_motor.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_manager.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "string.hpp"

namespace manager {
	class StepperMotorManager: public InventoryManager<StepperMotor> {
	public:
		StepperMotorManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, Inventory<server::ResourceId, Gpio> *gpio_inventory);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;
	private:
		static StepperMotor *create_stepper_motor(Inventory<server::ResourceId, Gpio> *gpio_inventory, const server::Data& cfg);
		static server::Object read_stepper_motor(const StepperMotor& stepper_motor);
		static void write_stepper_motor(StepperMotor *stepper_motor, const server::Data& update_cfg);

		using ControlOutput = typename manager_utl::GpoStepperMotor::ControlOutput;
		using ControlOutputs = typename manager_utl::GpoStepperMotor::ControlOutputs;
		using DirectionOutput = typename manager_utl::GpoStepperMotor::DirectionOutput;
		using DirectionOutputs = typename manager_utl::GpoStepperMotor::DirectionOutputs;
		using MotorState = typename manager_utl::GpoStepperMotor::MotorState;
		using MotorStates = typename manager_utl::GpoStepperMotor::MotorStates;
		
		template <typename Toutputs, typename Toutput>
		static Toutputs fetch_outputs(Inventory<server::ResourceId, Gpio> *inventory, const server::Object& cfg, const std::map<std::string, Toutput>& fields_mapping);
		
		static MotorState parse_state(const server::Object& cfg, const std::map<std::string, DirectionOutput>& fields_mapping);	
	};
	
	inline StepperMotor *StepperMotorManager::create_stepper_motor(Inventory<server::ResourceId, Gpio> *gpio_inventory, const server::Data& cfg) {
		using namespace server;
		using namespace manager_utl;

		if (nullptr == gpio_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
		const auto& cfg_obj = Data::cast<Object>(cfg);

		static const std::map<std::string, ControlOutput> str_to_control_output_mapping {
			{"ena", ControlOutput::ENA},
			{"enb", ControlOutput::ENB},
		};
		const ControlOutputs control_outputs = fetch_outputs<ControlOutputs, ControlOutput>(
			gpio_inventory,
			Data::cast<Object>(cfg_obj.access("control_outputs")),
			str_to_control_output_mapping
		);

		static const std::map<std::string, DirectionOutput> str_to_direction_output_mapping {
			{"a_top", DirectionOutput::A_TOP},
			{"a_btm", DirectionOutput::A_BTM},
			{"b_top", DirectionOutput::B_TOP},
			{"b_btm", DirectionOutput::B_BTM},
		};
		const DirectionOutputs direction_outputs = fetch_outputs<DirectionOutputs, DirectionOutput>(
			gpio_inventory,
			Data::cast<Object>(cfg_obj.access("direction_outputs")),
			str_to_direction_output_mapping
		);

		const auto& states_cfg = Data::cast<Array>(cfg_obj.access("states"));
		MotorStates states;
		states.reserve(states.size());
		states_cfg.for_each(
			[&states](int, const Data& state_cfg) {
				states.push_back(
					parse_state(
						Data::cast<Object>(state_cfg), 
						str_to_direction_output_mapping
					)
				);
			}
		);
		return new GpoStepperMotor(
			control_outputs,
			direction_outputs,
			states
		);
	}

	inline StepperMotorManager::StepperMotorManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, Inventory<server::ResourceId, Gpio> *gpio_inventory): InventoryManager<StepperMotor>(stepper_motor_inventory, [gpio_inventory](const server::Data& cfg) {return create_stepper_motor(gpio_inventory, cfg);}, read_stepper_motor, write_stepper_motor) {

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
			if (enable_status) {
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
			return;
		}
		throw ServerException(ResponseCode::BAD_REQUEST, "wrong update type: " + update_type);
	}

	template <typename Toutputs, typename Toutput>
	inline Toutputs StepperMotorManager::fetch_outputs(Inventory<server::ResourceId, Gpio> *inventory, const server::Object& cfg, const std::map<std::string, Toutput>& fields_mapping) {
		using namespace server;
		Toutputs result;
		cfg.for_each(
			[&result, fields_mapping, inventory](const std::string& output_str, const Data& gpo_id) {
				const auto output = fields_mapping.at(output_str);
				const auto gpo_resourse_id = static_cast<ResourceId>(Data::cast<String>(gpo_id).get());
				auto& gpo_ref = Gpio::cast<Gpo>(inventory->access(gpo_resourse_id));
				result.insert({output, &gpo_ref});
			}
		);
		return result;
	}

	inline typename StepperMotorManager::MotorState StepperMotorManager::parse_state(const server::Object& cfg, const std::map<std::string, DirectionOutput>& fields_mapping) {
		using namespace server;
		MotorState result;
		cfg.for_each(
			[&result, fields_mapping](const std::string& output_str, const Data& gpo_state) {
				const auto output = fields_mapping.at(output_str);
				const auto state = static_cast<Gpo::State>(Data::cast<Integer>(gpo_state).get());
				result.insert({output, state});
			}
		);
		return result;
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP