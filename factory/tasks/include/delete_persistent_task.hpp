#ifndef	DELETE_PERSISTENT_TASK_HPP
#define	DELETE_PERSISTENT_TASK_HPP

#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Ttask_id>
	class DeletePersistentTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using PersistentTask = mcu_server::Task<mcu_server::Data *(void)>;
		using PersistentTaskInventory = mcu_platform::Inventory<Ttask_id, PersistentTask>;
		using ReportCreator = mcu_server::Creator<mcu_server::Data *(int)>;
		
		DeletePersistentTask(PersistentTaskInventory *inventory, const Ttask_id& id, const ReportCreator& report_ctor);
		DeletePersistentTask(const DeletePersistentTask& other) = delete;
		DeletePersistentTask& operator=(const DeletePersistentTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		PersistentTaskInventory * const m_inventory;
		const Ttask_id m_id;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Ttask_id>
	inline DeletePersistentTask<Ttask_id>::DeletePersistentTask(PersistentTaskInventory *inventory, const Ttask_id& id, const ReportCreator& report_ctor): m_inventory(inventory), m_id(id), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Ttask_id>
	inline mcu_server::Data *DeletePersistentTask<Ttask_id>::execute() const {
		auto gpio_ptr = m_inventory->pull(m_id);
		delete gpio_ptr;
		gpio_ptr = nullptr;
		return m_report_ctor->create(0);
	}
}
#endif // DELETE_PERSISTENT_TASK_HPP