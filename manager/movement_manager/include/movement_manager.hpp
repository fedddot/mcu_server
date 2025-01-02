#ifndef	MOVEMENT_MANAGER_HPP
#define	MOVEMENT_MANAGER_HPP

#include "data.hpp"
#include "movement.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_manager.hpp"
#include "object.hpp"
#include "server_types.hpp"

namespace manager {
	class MovementManager: public InventoryManager<Movement> {
	public:
		using MovementCreator = typename InventoryManager<Movement>::ItemCreator;
		MovementManager(Inventory<server::ResourceId, Movement> *movement_inventory, const MovementCreator& movement_creator);
		MovementManager(const MovementManager& other) = delete;
		MovementManager& operator=(const MovementManager&) = delete;
	private:
		static server::Object read_movement(const Movement& movement);
		static void write_movement(Movement *movement, const manager::Data& update_cfg);
	};
	
	inline MovementManager::MovementManager(Inventory<server::ResourceId, Movement> *movement_inventory, const MovementCreator& movement_creator): InventoryManager<Movement>(movement_inventory, movement_creator, read_movement, write_movement) {

	}

	inline server::Object MovementManager::read_movement(const Movement& movement) {
		using namespace server;
		Object movement_data;
		movement_data.add("type", Integer(static_cast<int>(movement.type())));
		return movement_data;
	}
	
	inline void MovementManager::write_movement(Movement *movement, const manager::Data& update_cfg) {
		using namespace server;
		movement->perform(update_cfg);
	}
}

#endif // MOVEMENT_MANAGER_HPP