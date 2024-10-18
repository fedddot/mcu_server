#ifndef	MOVEMENT_MANAGER_HPP
#define	MOVEMENT_MANAGER_HPP

#include <functional>
#include <stdexcept>
#include <string>

#include "array.hpp"
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
	class MovementManager: public server::Manager {
	public:
		MovementManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory);
		MovementManager(const MovementManager& other) = delete;
		MovementManager& operator=(const MovementManager&) = delete;

		void create_resource(const server::Body& create_request_body) override;
		server::Body read_resource(const server::Path& route) const override;
		server::Body read_all_resources() const override;
		void update_resource(const server::Path& route, const server::Body& update_request_body) override;
		void delete_resource(const server::Path& route) override;
		bool contains(const server::Path& route) const override;
	private:
		Inventory<server::ResourceId, StepperMotor> *m_stepper_motor_inventory;
		void run_movement(const server::Object& movement_cfg);
	};
	
	inline MovementManager::MovementManager(Inventory<server::ResourceId, StepperMotor> *stepper_motor_inventory): m_stepper_motor_inventory(stepper_motor_inventory) {
		if (!m_stepper_motor_inventory) {
			throw std::invalid_argument("invalid arguments received");
		}
	}
	
	inline void MovementManager::create_resource(const server::Body& create_request_body) {
		using namespace server;
		const auto& config(Data::cast<Object>(create_request_body.access("config")));
		const auto& movements(Data::cast<Array>(config.access("movements")));
		const auto repeats(static_cast<unsigned int>(Data::cast<Integer>(config.access("repeats")).get()));
		for (auto i = 0; repeats > i; ++i) {
			movements.for_each(
				[this](int, const Data& movement_cfg) {
					run_movement(Data::cast<Object>(movement_cfg));
				}
			);
		}
	}
	
	inline server::Body MovementManager::read_resource(const server::Path& route) const {
		using namespace server;
		throw ServerException(ResponseCode::METHOD_NOT_ALLOWED, "method not supported");
	}

	inline server::Body MovementManager::read_all_resources() const {
		using namespace server;
		throw ServerException(ResponseCode::METHOD_NOT_ALLOWED, "method not supported");
	}

	inline void MovementManager::update_resource(const server::Path& route, const server::Body& update_body) {
		using namespace server;
		throw ServerException(ResponseCode::METHOD_NOT_ALLOWED, "method not supported");
	}
	
	inline void MovementManager::delete_resource(const server::Path& route) {
		using namespace server;
		throw ServerException(ResponseCode::METHOD_NOT_ALLOWED, "method not supported");
	}

	inline bool MovementManager::contains(const server::Path& route) const {
		using namespace server;
		throw ServerException(ResponseCode::METHOD_NOT_ALLOWED, "method not supported");
	}

	inline void MovementManager::run_movement(const server::Object& movement_cfg) {
		using namespace server;
		const auto motor_id(static_cast<ResourceId>(Data::cast<String>(movement_cfg.access("motor_id")).get()));
		const auto& steps_cfg(Data::cast<Object>(movement_cfg.access("steps_cfg")));
		const auto direction(static_cast<StepperMotor::Direction>(Data::cast<Integer>(steps_cfg.access("dir")).get()));
		const auto steps_num(static_cast<unsigned int>(Data::cast<Integer>(steps_cfg.access("steps_num")).get()));
		const auto on_time(static_cast<unsigned int>(Data::cast<Integer>(steps_cfg.access("on_time")).get()));
		const auto off_time(static_cast<unsigned int>(Data::cast<Integer>(steps_cfg.access("off_time")).get()));
		(m_stepper_motor_inventory->access(motor_id)).steps(direction, steps_num, on_time, off_time);
	}
}

#endif // MOVEMENT_MANAGER_HPP