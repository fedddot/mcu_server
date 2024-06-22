#ifndef	TASK_FACTORY_HPP
#define	TASK_FACTORY_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "parser.hpp"
#include "task.hpp"

namespace server_utl {
	template <class Tdata, class Treport, class Ttask_id>
	class TaskFactory: public engine::Creator<engine::Task<Treport> *(const Tdata&)> {
	public:
		using Task = engine::Task<Treport>;
		using TaskCreator = engine::Creator<Task *(const Tdata&)>;
		using TaskIdParser = engine::Parser<Ttask_id(const Tdata&)>;
		
		TaskFactory(const TaskIdParser& task_id_parser);
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;

		Task *create(const Tdata& cfg) const override;
		engine::Creator<Task *(const Tdata&)> *clone() const override;

		void register_creator(const Ttask_id& id, const TaskCreator& creator);
	private:
		std::shared_ptr<TaskIdParser> m_task_id_parser;
		std::map<Ttask_id, std::shared_ptr<TaskCreator>> m_creators;
	};

	template <class Tdata, class Treport, class Ttask_id>
	inline TaskFactory<Tdata, Treport, Ttask_id>::TaskFactory(const TaskIdParser& task_id_parser): m_task_id_parser(task_id_parser.clone()) {

	}

	template <class Tdata, class Treport, class Ttask_id>
	inline void TaskFactory<Tdata, Treport, Ttask_id>::register_creator(const Ttask_id& id, const TaskCreator& creator) {
		m_creators[id] = std::shared_ptr<TaskCreator>(creator.clone());
	}

	template <class Tdata, class Treport, class Ttask_id>
	inline typename TaskFactory<Tdata, Treport, Ttask_id>::Task *TaskFactory<Tdata, Treport, Ttask_id>::create(const Tdata& cfg) const {
		auto iter = m_creators.find(m_task_id_parser->parse(cfg));
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator with received type not found");
		}
		return (iter->second)->create(cfg);
	}

	template <class Tdata, class Treport, class Ttask_id>
	inline engine::Creator<engine::Task<Treport> *(const Tdata&)> *TaskFactory<Tdata, Treport, Ttask_id>::clone() const {
		return new TaskFactory<Tdata, Treport, Ttask_id>(*this);
	}
}
#endif // TASK_FACTORY_HPP