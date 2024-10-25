#ifndef	CNC_SERVER_HPP
#define	CNC_SERVER_HPP

#include <memory>
#include <stdexcept>
#include "clonable_manager_wrapper.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "linear_movement.hpp"
#include "movement.hpp"
#include "movement_manager.hpp"
#include "object.hpp"
#include "server.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
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
			server::Vendor *vendor,
			const GpioCreator& gpio_creator,
			const StepperMotorCreator& stepper_motor_creator,
			const DelayFunction& delay_function
		);
		CncServer(const CncServer& other) = delete;
		CncServer& operator=(const CncServer& other) = delete;

		void run();
		bool is_running() const;
		void stop();
	private:
		std::unique_ptr<server::Vendor> m_vendor;

		manager::InMemoryInventory<server::ResourceId, manager::Gpio> m_gpio_inventory;
		GpioCreator m_gpio_creator;
		
		manager::InMemoryInventory<server::ResourceId, manager::StepperMotor> m_stepper_motor_inventory;
		StepperMotorCreator m_stepper_motor_creator;
		
		manager::InMemoryInventory<server::ResourceId, manager::Movement> m_movement_inventory;

		DelayFunction m_delay_function;
		
		
		server::Vendor *init_vendor();
		manager::Movement *create_movement(const server::Data& cfg) const;
	};

	template <typename Tsubscriber_id>
	inline CncServer<Tsubscriber_id>::CncServer(IpcConnection *connection, const Tsubscriber_id& id, server::Vendor *vendor): server::Server<Tsubscriber_id>(connection, id, init_vendor()) {

	}

	template <typename Tsubscriber_id>
	inline server::Vendor *CncServer<Tsubscriber_id>::init_vendor() {
		using namespace server;
		using namespace vendor;
		using namespace manager;

		ClonableManagerWrapper gpio_manager(
			new GpioManager(
				&m_gpio_inventory,
				m_gpio_creator
			)
		);
		ClonableManagerWrapper stepper_motor_manager(
			new StepperMotorManager(
				&m_stepper_motor_inventory,
				m_stepper_motor_creator
			)
		);
		ClonableManagerWrapper movement_manager(
			new MovementManager(
				&m_movement_inventory,
				[this](const Data& cfg) {
					return create_movement(cfg); 
				}
			)
		);
		m_vendor = std::unique_ptr<Vendor>(new ResourcesVendor());
		auto& vendor_ref = dynamic_cast<ResourcesVendor&>(*m_vendor);
		vendor_ref.add_manager("gpios", gpio_manager);
		vendor_ref.add_manager("steppers", stepper_motor_manager);
		vendor_ref.add_manager("movements", movement_manager);
		return m_vendor.get();
	}

	template <typename Tsubscriber_id>
	inline manager::Movement *CncServer<Tsubscriber_id>::create_movement(const server::Data& cfg) const {
		using namespace server;
		using namespace manager;

		const auto& cfg_obj(Data::cast<Object>(cfg));
		const auto movement_type(static_cast<Movement::Type>(Data::cast<Integer>(cfg_obj.access("type")).get()));
		auto parse_motors_assignments = [](const Data& data)-> LinearMovement::AxesAssignment {

		};
		
		switch (movement_type) {
		case Movement::Type::LINEAR:
			return new LinearMovement(
				&m_stepper_motor_inventory,
				m_delay_function,
				parse_motors_assignments(cfg_obj.access("steppers"))
			);
		default:
			throw ServerException(ResponseCode::BAD_REQUEST, "movement type is not supported");
		}
	}
}

#endif // CNC_SERVER_HPP