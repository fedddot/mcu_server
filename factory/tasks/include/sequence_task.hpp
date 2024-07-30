#ifndef	SEQUENCE_TASK_HPP
#define	SEQUENCE_TASK_HPP

#include <memory>
#include <vector>

#include "array.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "task.hpp"

namespace mcu_factory {

	class SequenceTask: public mcu_server::Task<mcu_server::Data *(void)> {
	public:
		using SubTask = mcu_server::Task<mcu_server::Data *(void)>;
		using TaskCreator = mcu_server::Creator<SubTask *(const mcu_server::Data&)>;
		using ReportCreator = mcu_server::Creator<mcu_server::Data *(const mcu_server::Array&)>;
		
		SequenceTask(const TaskCreator& task_ctor, const mcu_server::Array& tasks_data, const ReportCreator& report_ctor);
		SequenceTask(const SequenceTask& other) = delete;
		SequenceTask& operator=(const SequenceTask& other) = delete;
		
		mcu_server::Data *execute() const override;
	private:
		const std::unique_ptr<TaskCreator> m_task_ctor;
		const mcu_server::Array m_tasks_data;
		const std::unique_ptr<ReportCreator> m_report_ctor;
	};

	inline SequenceTask::SequenceTask(const TaskCreator& task_ctor, const mcu_server::Array& tasks_data, const ReportCreator& report_ctor): m_task_ctor(task_ctor.clone()), m_tasks_data(tasks_data), m_report_ctor(report_ctor.clone()) {

	}

	inline mcu_server::Data *SequenceTask::execute() const {
		std::vector<std::unique_ptr<SubTask>> tasks;
		m_tasks_data.for_each(
			[&tasks, this](int index, const mcu_server::Data& subtask_data) {
				(void)index;
				tasks.push_back(std::unique_ptr<SubTask>(m_task_ctor->create(subtask_data)));
			}
		);
		mcu_server::Array reports;
		for (auto& task: tasks) {
			std::unique_ptr<mcu_server::Data> report(task->execute());
			reports.push_back(*report);
		}
		return m_report_ctor->create(reports);
	}
}
#endif // SEQUENCE_TASK_HPP