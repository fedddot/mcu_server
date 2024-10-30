#ifndef	CNC_SERVER_HPP
#define	CNC_SERVER_HPP

#include <memory>
#include <stdexcept>
#include <string>

#include "clonable_manager_wrapper.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "linear_movement.hpp"
#include "movement.hpp"
#include "movement_manager.hpp"
#include "movement_types.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "string.hpp"
#include "vendor.hpp"

namespace cnc_server {
	template <typename Tsubscriber_id>
	class CncServer: public server::Server<Tsubscriber_id> {
	public:
		using IpcConnection = typename server::Server<Tsubscriber_id>::IpcConnection;
		using GpioCreator = typename manager::GpioManager::GpioCreator;
		using StepperMotorCreator = typename manager::StepperMotorManager::StepperMotorCreator;
		using DelayFunction = typename manager::LinearMovement::DelayFunction;

		CncServer(
			IpcConnection *connection,
			const Tsubscriber_id& id,
			const GpioCreator& gpio_creator,
			const StepperMotorCreator& stepper_motor_creator,
			const DelayFunction& delay_function
		);
		CncServer(const CncServer& other) = delete;
		CncServer& operator=(const CncServer& other) = delete;
	private:
		std::shared_ptr<server::Vendor> m_vendor;

		manager::InMemoryInventory<server::ResourceId, manager::Gpio> m_gpio_inventory;
		
		manager::InMemoryInventory<server::ResourceId, manager::StepperMotor> m_stepper_motor_inventory;
		
		manager::InMemoryInventory<server::ResourceId, manager::Movement> m_movement_inventory;

		server::Vendor *init_vendor(const GpioCreator& gpio_creator, const StepperMotorCreator& stepper_motor_creator, const DelayFunction& delay_function);
		manager::Movement *create_movement(const server::Data& cfg, const DelayFunction& delay_function);
	};

	template <typename Tsubscriber_id>
	inline CncServer<Tsubscriber_id>::CncServer(
		IpcConnection *connection,
		const Tsubscriber_id& id,
		const GpioCreator& gpio_creator,
		const StepperMotorCreator& stepper_motor_creator,
		const DelayFunction& delay_function
	): server::Server<Tsubscriber_id>(connection, id, init_vendor(gpio_creator, stepper_motor_creator, delay_function)) {
	}

	template <typename Tsubscriber_id>
	inline server::Vendor *CncServer<Tsubscriber_id>::init_vendor(const GpioCreator& gpio_creator, const StepperMotorCreator& stepper_motor_creator, const DelayFunction& delay_function) {
		using namespace server;
		using namespace vendor;
		using namespace manager;
		if (!gpio_creator || !stepper_motor_creator || !delay_function) {
			throw std::invalid_argument("invalid actions received");
		}
		ClonableManagerWrapper gpio_manager(
			new GpioManager(
				&m_gpio_inventory,
				gpio_creator
			)
		);
		ClonableManagerWrapper stepper_motor_manager(
			new StepperMotorManager(
				&m_stepper_motor_inventory,
				stepper_motor_creator
			)
		);
		ClonableManagerWrapper movement_manager(
			new MovementManager(
				&m_movement_inventory,
				[this, &delay_function](const Data& cfg) {
					return create_movement(cfg, delay_function); 
				}
			)
		);
		m_vendor = std::shared_ptr<Vendor>(new ResourcesVendor());
		auto& vendor_ref = dynamic_cast<ResourcesVendor&>(*m_vendor);
		vendor_ref.add_manager("gpios", gpio_manager);
		vendor_ref.add_manager("steppers", stepper_motor_manager);
		vendor_ref.add_manager("movements", movement_manager);
		return m_vendor.get();
	}

	template <typename Tsubscriber_id>
	inline manager::Movement *CncServer<Tsubscriber_id>::create_movement(const server::Data& cfg, const DelayFunction& delay_function) {
		using namespace server;
		using namespace manager;

		const auto& cfg_obj(Data::cast<Object>(cfg));
		const auto movement_type(static_cast<Movement::Type>(Data::cast<Integer>(cfg_obj.access("type")).get()));
		auto str_to_axis = [](const std::string& tag) {
			if ("x" == tag) {
				return Axis::X;
			} else if ("y" == tag) {
				return Axis::Y;
			} else if ("z" == tag) {
				return Axis::Z;
			}
			throw ServerException(ResponseCode::BAD_REQUEST, "invalid axis tag received: " + tag);
		};
		auto parse_motors_assignments = [str_to_axis](const Data& data) {
			LinearMovement::AxesAssignment assignment;
			Data::cast<Object>(data).for_each(
				[&assignment, str_to_axis](const std::string& tag, const Data& gpio_id) {
					assignment.insert({str_to_axis(tag), static_cast<ResourceId>(Data::cast<String>(gpio_id).get())});
				}
			);
			return assignment;
		};
		auto parse_time_multiplier = [](const Data& data)-> unsigned int {
			return static_cast<unsigned int>(Data::cast<Integer>(data).get());
		};
		
		switch (movement_type) {
		case Movement::Type::LINEAR:
			return new LinearMovement(
				&m_stepper_motor_inventory,
				delay_function,
				parse_motors_assignments(cfg_obj.access("steppers")),
				parse_time_multiplier(cfg_obj.access("time_multiplier"))
			);
		default:
			throw ServerException(ResponseCode::BAD_REQUEST, "movement type is not supported");
		}
	}
}

#endif // CNC_SERVER_HPP