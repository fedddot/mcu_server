#ifndef	CREATE_PERSISTENT_TASK_HPP
#define	CREATE_PERSISTENT_TASK_HPP

#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Ttask_id>
	class CreatePersistentTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using PersistentTask = mcu_server::Task<mcu_server::Data *(void)>;
		using PersistentTaskInventory = mcu_platform::Inventory<Ttask_id, PersistentTask>;
		using TaskCreator = mcu_server::Creator<PersistentTask *(const mcu_server::Data&)>;
		using ReportCreator = mcu_server::Creator<mcu_server::Data *(int)>;
		
		CreatePersistentTask(PersistentTaskInventory *inventory, const Ttask_id& id, const TaskCreator& task_ctor, const mcu_server::Data& data, const ReportCreator& report_ctor);
		CreatePersistentTask(const CreatePersistentTask& other) = delete;
		CreatePersistentTask& operator=(const CreatePersistentTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		PersistentTaskInventory * const m_inventory;
		const Ttask_id m_id;
		const std::unique_ptr<TaskCreator> m_task_ctor;
		const std::unique_ptr<mcu_server::Data> m_data;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	template <class Ttask_id>
	inline CreatePersistentTask<Ttask_id>::CreatePersistentTask(PersistentTaskInventory *inventory, const Ttask_id& id, const TaskCreator& task_ctor, const mcu_server::Data& data, const ReportCreator& report_ctor): m_inventory(inventory), m_id(id), m_task_ctor(task_ctor.clone()), m_data(data.clone()), m_report_ctor(report_ctor.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Ttask_id>
	inline mcu_server::Data *CreatePersistentTask<Ttask_id>::execute() const {
		auto task_ptr = m_task_ctor->create(*m_data);
		try {
			m_inventory->put(m_id, task_ptr);
		} catch (...) {
			delete task_ptr;
			task_ptr = nullptr;
			throw;
		}
		return m_report_ctor->create(0);
	}
}
#endif // CREATE_PERSISTENT_TASK_HPP