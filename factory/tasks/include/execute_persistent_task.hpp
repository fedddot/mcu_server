#ifndef	EXECUTE_PERSISTENT_TASK_HPP
#define	EXECUTE_PERSISTENT_TASK_HPP

#include <stdexcept>

#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Ttask_id>
	class ExecutePersistentTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using PersistentTask = mcu_server::Task<mcu_server::Data *(void)>;
		using PersistentTaskInventory = mcu_platform::Inventory<Ttask_id, PersistentTask>;
		
		ExecutePersistentTask(PersistentTaskInventory *inventory, const Ttask_id& id);
		ExecutePersistentTask(const ExecutePersistentTask& other) = delete;
		ExecutePersistentTask& operator=(const ExecutePersistentTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		PersistentTaskInventory * const m_inventory;
		const Ttask_id m_id;
	};

	template <class Ttask_id>
	inline ExecutePersistentTask<Ttask_id>::ExecutePersistentTask(PersistentTaskInventory *inventory, const Ttask_id& id): m_inventory(inventory), m_id(id) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Ttask_id>
	inline mcu_server::Data *ExecutePersistentTask<Ttask_id>::execute() const {
		return m_inventory->access(m_id)->execute();
	}
}
#endif // EXECUTE_PERSISTENT_TASK_HPP