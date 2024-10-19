#ifndef	MOVEMENT_MANAGER_HPP
#define	MOVEMENT_MANAGER_HPP

#include <functional>
#include <stdexcept>
#include <string>

#include "movement.hpp"
#include "object.hpp"
#include "inventory.hpp"
#include "manager.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "string.hpp"

namespace manager {
	class MovementManager: public server::Manager {
	public:
		using MovementCreator = std::function<Movement *(const server::Body&)>;
		using MovementReader = std::function<server::Body(const Movement&)>;
		MovementManager(Inventory<server::ResourceId, Movement> *movement_inventory, const MovementCreator& movement_creator, const MovementReader& movement_reader);
		MovementManager(const MovementManager& other) = delete;
		MovementManager& operator=(const MovementManager&) = delete;

		void create_resource(const server::Body& create_request_body) override;
		server::Body read_resource(const server::Path& route) const override;
		server::Body read_all_resources() const override;
		void update_resource(const server::Path& route, const server::Body& update_request_body) override;
		void delete_resource(const server::Path& route) override;
		bool contains(const server::Path& route) const override;
	private:
		Inventory<server::ResourceId, Movement> *m_movement_inventory;
		MovementCreator m_movement_creator;
		MovementReader m_movement_reader;
		void run_movement(const server::Object& movement_cfg);
		static server::ResourceId get_id_from_route(const server::Path& route);
	};
	
	inline MovementManager::MovementManager(Inventory<server::ResourceId, Movement> *movement_inventory, const MovementCreator& movement_creator, const MovementReader& movement_reader): m_movement_inventory(movement_inventory), m_movement_creator(movement_creator), m_movement_reader(movement_reader) {
		if (!m_movement_inventory) {
			throw std::invalid_argument("invalid arguments received");
		}
	}
	
	inline void MovementManager::create_resource(const server::Body& create_request_body) {
		using namespace server;
		auto id(static_cast<ResourceId>(Data::cast<String>(create_request_body.access("id")).get()));
		if (m_movement_inventory->contains(id)) {
			throw ServerException(ResponseCode::BAD_REQUEST, "movement with id " + id + " already exists");
		}
		m_movement_inventory->add(id, m_movement_creator(create_request_body));
	}
	
	inline server::Body MovementManager::read_resource(const server::Path& route) const {
		using namespace server;
		const auto movement_id(get_id_from_route(route));
		if (!(m_movement_inventory->contains(movement_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "movement with specified id doesn't exist");
		}
		const auto& movement(m_movement_inventory->access(movement_id));
		return m_movement_reader(movement);
	}

	inline server::Body MovementManager::read_all_resources() const {
		using namespace server;
		Body response_body;
		for (const auto& id: m_movement_inventory->ids()) {
			response_body.add(id, read_resource({id}));
		}
		return response_body;
	}

	inline void MovementManager::update_resource(const server::Path& route, const server::Body& update_body) {
		using namespace server;
		const auto movement_id(get_id_from_route(route));
		if (!(m_movement_inventory->contains(movement_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "movement with specified id doesn't exist");
		}
		const auto& movement_config(update_body.access("config"));
		(m_movement_inventory->access(movement_id)).perform(movement_config);
	}

	inline void MovementManager::delete_resource(const server::Path& route) {
		using namespace server;
		const auto movement_id(get_id_from_route(route));
		if (!(m_movement_inventory->contains(movement_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "movement with specified id doesn't exist");
		}
		std::unique_ptr<Movement> movement_ptr(m_movement_inventory->remove(movement_id));
		movement_ptr = nullptr;
	}

	inline bool MovementManager::contains(const server::Path& route) const {
		using namespace server;
		return m_movement_inventory->contains(get_id_from_route(route));
	}

	inline server::ResourceId MovementManager::get_id_from_route(const server::Path& route) {
		using namespace server;
		if (1UL != route.size()) {
			throw ServerException(ResponseCode::BAD_REQUEST, "bad route received");
		}
		return route[0];
	}
}

#endif // MOVEMENT_MANAGER_HPP