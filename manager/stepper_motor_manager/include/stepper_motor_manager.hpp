#ifndef	STEPPER_MOTOR_MANAGER_HPP
#define	STEPPER_MOTOR_MANAGER_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

#include "integer.hpp"
#include "object.hpp"
#include "stepper_motor.hpp"
#include "inventory.hpp"
#include "manager.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "stepper_motor.hpp"
#include "string.hpp"

namespace manager {
	class StepperMotorManager: public server::Manager {
	public:
		using StepperMotorCreator = std::function<StepperMotor *(const server::Body&)>;
		using StepperMotorReader = std::function<server::Body(const StepperMotor&)>;
		StepperMotorManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const StepperMotorCreator& stepper_motor_creator, const StepperMotorReader& stepper_motor_reader);
		StepperMotorManager(const StepperMotorManager& other) = delete;
		StepperMotorManager& operator=(const StepperMotorManager&) = delete;

		void create_resource(const server::Body& create_request_body) override;
		server::Body read_resource(const server::Path& route) const override;
		server::Body read_all_resources() const override;
		void update_resource(const server::Path& route, const server::Body& update_request_body) override;
		void delete_resource(const server::Path& route) override;
		bool contains(const server::Path& route) const override;
	private:
		Inventory<server::ResourceId, StepperMotor> *m_stepper_motor_inventory;
		StepperMotorCreator m_stepper_motor_creator;
		StepperMotorReader m_stepper_motor_reader;
		static server::ResourceId get_id_from_route(const server::Path& route);
	};
	
	inline StepperMotorManager::StepperMotorManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory, const StepperMotorCreator& stepper_motor_creator, const StepperMotorReader& stepper_motor_reader): m_stepper_motor_inventory(stepper_motor_inventory), m_stepper_motor_creator(stepper_motor_creator), m_stepper_motor_reader(stepper_motor_reader) {
		if (!m_stepper_motor_inventory || !m_stepper_motor_creator || !m_stepper_motor_reader) {
			throw std::invalid_argument("invalid arguments received");
		}
	}
	
	inline void StepperMotorManager::create_resource(const server::Body& create_request_body) {
		using namespace server;
		auto id(static_cast<ResourceId>(Data::cast<String>(create_request_body.access("id")).get()));
		if (m_stepper_motor_inventory->contains(id)) {
			throw ServerException(ResponseCode::BAD_REQUEST, "stepper_motor with id " + id + " already exists");
		}
		m_stepper_motor_inventory->add(id, m_stepper_motor_creator(create_request_body));
	}
	
	inline server::Body StepperMotorManager::read_resource(const server::Path& route) const {
		using namespace server;
		const auto stepper_motor_id(get_id_from_route(route));
		if (!(m_stepper_motor_inventory->contains(stepper_motor_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "stepper_motor with specified id doesn't exist");
		}
		const auto& gpio(m_stepper_motor_inventory->access(stepper_motor_id));
		return m_stepper_motor_reader(gpio);
	}

	inline server::Body StepperMotorManager::read_all_resources() const {
		using namespace server;
		Body response_body;
		for (const auto& id: m_stepper_motor_inventory->ids()) {
			response_body.add(id, read_resource({id}));
		}
		return response_body;
	}

	inline void StepperMotorManager::update_resource(const server::Path& route, const server::Body& update_body) {
		using namespace server;
		const auto stepper_motor_id(get_id_from_route(route));
		if (!(m_stepper_motor_inventory->contains(stepper_motor_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "stepper_motor with specified id doesn't exist");
		}
		const auto& config(Data::cast<Object>(update_body.access("config")));
		const auto direction(static_cast<StepperMotor::Direction>(Data::cast<Integer>(config.access("dir")).get()));
		const auto steps_num(static_cast<unsigned int>(Data::cast<Integer>(config.access("steps_num")).get()));
		const auto on_time(static_cast<unsigned int>(Data::cast<Integer>(config.access("on_time")).get()));
		const auto off_time(static_cast<unsigned int>(Data::cast<Integer>(config.access("off_time")).get()));
		(m_stepper_motor_inventory->access(stepper_motor_id)).steps(direction, steps_num, on_time, off_time);
	}
	
	inline void StepperMotorManager::delete_resource(const server::Path& route) {
		using namespace server;
		const auto stepper_motor_id(get_id_from_route(route));
		if (!(m_stepper_motor_inventory->contains(stepper_motor_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "stepper_motor with specified id doesn't exist");
		}
		std::unique_ptr<StepperMotor> stepper_motor_ptr(m_stepper_motor_inventory->remove(stepper_motor_id));
		stepper_motor_ptr = nullptr;
	}

	inline bool StepperMotorManager::contains(const server::Path& route) const {
		using namespace server;
		return m_stepper_motor_inventory->contains(get_id_from_route(route));
	}

	inline server::ResourceId StepperMotorManager::get_id_from_route(const server::Path& route) {
		using namespace server;
		if (1UL != route.size()) {
			throw ServerException(ResponseCode::BAD_REQUEST, "bad route received");
		}
		return route[0];
	}
}

#endif // STEPPER_MOTOR_MANAGER_HPP