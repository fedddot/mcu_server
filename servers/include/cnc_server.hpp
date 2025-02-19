#ifndef	CNC_SERVER_HPP
#define	CNC_SERVER_HPP

#include <memory>
#include <stdexcept>
#include <string>

#include "circular_movement.hpp"
#include "clonable_manager_wrapper.hpp"
#include "custom_server.hpp"
#include "data.hpp"
#include "data_reader.hpp"
#include "data_writer.hpp"
#include "gpio.hpp"
#include "gpio_manager.hpp"
#include "in_memory_inventory.hpp"
#include "integer.hpp"
#include "linear_movement.hpp"
#include "movement.hpp"
#include "movement_manager.hpp"
#include "object.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_manager.hpp"
#include "string.hpp"
#include "vector.hpp"
#include "vendor.hpp"

namespace cnc_server {
	class CncServer: public server::Server {
	public:
		using RequestReader = ipc::DataReader<server::Request>;
		using ResponseWriter = ipc::DataWriter<server::Response>;
		using GpioCreator = typename manager::GpioManager::GpioCreator;
		using DelayFunction = typename manager::LinearMovement::DelayFunction;

		CncServer(
			RequestReader *request_reader,
			ResponseWriter *response_writer,
			const GpioCreator& gpio_creator,
			const DelayFunction& delay_function
		);
		CncServer(const CncServer& other) = delete;
		CncServer& operator=(const CncServer& other) = delete;

		void run() override;
		bool is_running() const override;
		void stop() override;
	private:
		std::shared_ptr<server::Vendor> m_vendor;
		manager::InMemoryInventory<server::ResourceId, manager::Gpio> m_gpio_inventory;
		manager::InMemoryInventory<server::ResourceId, manager::StepperMotor> m_stepper_motor_inventory;
		manager::InMemoryInventory<server::ResourceId, manager::Movement> m_movement_inventory;

		server::Vendor *init_vendor(const GpioCreator& gpio_creator, const DelayFunction& delay_function);
		manager::Movement *create_movement(const server::Data& cfg, const DelayFunction& delay_function);

		std::shared_ptr<server::Server> m_server;
	};
	
	inline CncServer::CncServer(
		RequestReader *request_reader,
		ResponseWriter *response_writer,
		const GpioCreator& gpio_creator,
		const DelayFunction& delay_function
	): m_server(new server_utl::CustomServer(request_reader, response_writer, init_vendor(gpio_creator, delay_function))) {
	
	}

	inline void CncServer::run() {
		m_server->run();
	}

	inline bool CncServer::is_running() const {
		return m_server->is_running();
	}

	inline void CncServer::stop() {
		m_server->stop();
	}

	inline server::Vendor *CncServer::init_vendor(const GpioCreator& gpio_creator, const DelayFunction& delay_function) {
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

	
	inline manager::Movement *CncServer::create_movement(const server::Data& cfg, const DelayFunction& delay_function) {
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

#endif // CNC_SERVER_HPP