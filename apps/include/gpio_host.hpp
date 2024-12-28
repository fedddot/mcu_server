#ifndef	GPIO_HOST_HPP
#define	GPIO_HOST_HPP

#include "gpio_app_types.hpp"
#include "gpio_ipc_factory.hpp"
#include "gpio_request.hpp"
#include "host.hpp"
#include "object.hpp"
#include <memory>
#include <stdexcept>
#include <string>

namespace gpio_app {
	template <typename Tgpio_id>
	class GpioHost: public mcu_app::Host<manager::GpioRequest<Tgpio_id>, manager::GpioResponseWrapper, ManagerConfig, ProviderConfig, IpcConfig> {
	public:
		GpioHost(const ManagerConfig& manager_cfg, const ProviderConfig& provider_cfg, const IpcConfig& ipc_cfg);
		GpioHost(const GpioHost& other) = delete;
		GpioHost& operator=(const GpioHost& other) = delete;
	private:
		using ParentHost = mcu_app::Host<manager::GpioRequest<Tgpio_id>, manager::GpioResponseWrapper, ManagerConfig, ProviderConfig, IpcConfig>;
	};
	
	template <typename Tgpio_id>
	inline GpioHost<Tgpio_id>::GpioHost(
		const ManagerConfig& manager_cfg,
		const ProviderConfig& provider_cfg,
		const IpcConfig& ipc_cfg
	): ParentHost(
		GpioIpcFactory<Tgpio_id>(),
		ipc_cfg,
		
	) {
	
	}

	inline void GpioHost::run() {
		m_server->run();
	}

	inline bool GpioHost::is_running() const {
		return m_server->is_running();
	}

	inline void GpioHost::stop() {
		m_server->stop();
	}

	inline server::Vendor *GpioHost::init_vendor(const GpioCreator& gpio_creator, const DelayFunction& delay_function) {
		using namespace server;
		using namespace vendor;
		using namespace manager;
		if (!gpio_creator || !delay_function) {
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
				&m_gpio_inventory	
			)
		);
		ClonableManagerWrapper movement_manager(
			new MovementManager(
				&m_movement_inventory,
				[this, delay_function](const Data& cfg) {
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

	
	inline manager::Movement *GpioHost::create_movement(const server::Data& cfg, const DelayFunction& delay_function) {
		using namespace server;
		using namespace manager;
		using Axis = typename Vector<double>::Axis;

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
		auto parse_steps_per_length = [](const Data& data)-> unsigned int {
			return static_cast<unsigned int>(Data::cast<Integer>(data).get());
		};
		
		switch (movement_type) {
		case Movement::Type::LINEAR:
			return new LinearMovement(
				&m_stepper_motor_inventory,
				delay_function,
				parse_motors_assignments(cfg_obj.access("steppers")),
				parse_steps_per_length(cfg_obj.access("steps_per_length"))
			);
		case Movement::Type::CIRCULAR:
			return new CircularMovement(
				&m_stepper_motor_inventory,
				delay_function,
				parse_motors_assignments(cfg_obj.access("steppers")),
				parse_steps_per_length(cfg_obj.access("steps_per_length"))
			);
		default:
			throw ServerException(ResponseCode::BAD_REQUEST, "movement type is not supported");
		}
	}
}

#endif // GPIO_HOST_HPP