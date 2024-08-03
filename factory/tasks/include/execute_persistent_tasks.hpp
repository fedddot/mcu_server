#ifndef	EXECUTE_PERSISTENT_TASKS_HPP
#define	EXECUTE_PERSISTENT_TASKS_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include "array.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace mcu_factory {
	template <typename Ttask_id>
	class ExecutePersistentTasks: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using PersistentTask = mcu_server::Task<mcu_server::Data *(void)>;
		using PersistentTaskInventory = mcu_platform::Inventory<Ttask_id, PersistentTask>;
		using PersistentTasksList = std::vector<Ttask_id>;
		using ReportCreator = mcu_server::Creator<mcu_server::Data *(const mcu_server::Array&)>;
		
		ExecutePersistentTasks(PersistentTaskInventory *inventory, const PersistentTasksList& tasks_list, const ReportCreator& reporter);
		ExecutePersistentTasks(const ExecutePersistentTasks& other) = delete;
		ExecutePersistentTasks& operator=(const ExecutePersistentTasks& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		PersistentTaskInventory * const m_inventory;
		const PersistentTasksList m_tasks_list;
		const std::unique_ptr<ReportCreator> m_reporter;
	};

	template <class Ttask_id>
	inline ExecutePersistentTasks<Ttask_id>::ExecutePersistentTasks(PersistentTaskInventory *inventory, const PersistentTasksList& tasks_list, const ReportCreator& reporter): m_inventory(inventory), m_tasks_list(tasks_list), m_reporter(reporter.clone()) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
	}

	template <class Ttask_id>
	inline mcu_server::Data *ExecutePersistentTasks<Ttask_id>::execute() const {
		using namespace mcu_server;
		Array reports;
		for (auto task_id: m_tasks_list) {
			auto task_ptr = m_inventory->access(task_id);
			std::unique_ptr<Data> report(task_ptr->execute());
			reports.push_back(*report);
		}
		return m_reporter->create(reports);
	}
}
#endif // EXECUTE_PERSISTENT_TASKS_HPP