#ifndef	INVENTORY_TASK_HPP
#define	INVENTORY_TASK_HPP

#include <functional>
#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace server {
	template <class Tkey, class Titem>
	class InventoryTask: public engine::Task<engine::Data *> {
	public:
		InventoryTask(Inventory<Tkey, Titem> *inventory, const engine::Data& task_data);
		InventoryTask(const InventoryTask& other) = delete;
		InventoryTask& operator=(const InventoryTask& other) = delete;
		const engine::Data& access_data() const;
		Inventory<Tkey, Titem> *access_inventory() const;
	private:
		Inventory<Tkey, Titem> *m_inventory;
		std::unique_ptr<engine::Data> m_task_data;
	};

	template <class Tkey, class Titem>
	inline InventoryTask<Tkey, Titem>::InventoryTask(Inventory<Tkey, Titem> *inventory, const engine::Data& task_data): m_inventory(inventory), m_task_data(task_data.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr");
		}
	}

	template <class Tkey, class Titem>
	inline const engine::Data& InventoryTask<Tkey, Titem>::access_data() const {
		return std::ref(*m_task_data);
	}

	template <class Tkey, class Titem>
	inline Inventory<Tkey, Titem> *InventoryTask<Tkey, Titem>::access_inventory() const {
		return m_inventory;
	}
}
#endif // INVENTORY_TASK_HPP