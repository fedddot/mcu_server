#ifndef	SERVER_FACTORY_HPP
#define	SERVER_FACTORY_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "parser.hpp"
#include "task.hpp"

namespace server_utl {
	template <class Tdata, class Treport, class Ttask_id>
	class ServerFactory: public engine::Creator<engine::Task<Treport> *(const Tdata&)> {
	public:
		using ServerTask = typename engine::Task<Treport>;
		using TaskCreator = engine::Creator<ServerTask *(const Tdata&)>;
		
		using TaskIdParser = server::Parser<Ttask_id(const Tdata&)>;
		
		ServerFactory(const TaskIdParser& task_id_parser);
		ServerFactory(const ServerFactory& other) = default;
		ServerFactory& operator=(const ServerFactory& other) = default;

		ServerTask *create(const Tdata& cfg) const override;
		engine::Creator<engine::Task<Treport> *(const Tdata&)> *clone() const override;

		void register_creator(const Ttask_id& id, const TaskCreator& creator);
	private:
		std::shared_ptr<TaskIdParser> m_task_id_parser;
		std::map<Ttask_id, std::shared_ptr<TaskCreator>> m_creators;
	};

	template <class Tdata, class Treport, class Ttask_id>
	inline ServerFactory<Tdata, Treport, Ttask_id>::ServerFactory(const TaskIdParser& task_id_parser): m_task_id_parser(task_id_parser.clone()) {

	}

	template <class Tdata, class Treport, class Ttask_id>
	inline void ServerFactory<Tdata, Treport, Ttask_id>::register_creator(const Ttask_id& id, const TaskCreator& creator) {
		m_creators[id] = std::shared_ptr<TaskCreator>(creator.clone());
	}

	template <class Tdata, class Treport, class Ttask_id>
	inline typename ServerFactory<Tdata, Treport, Ttask_id>::ServerTask *ServerFactory<Tdata, Treport, Ttask_id>::create(const Tdata& cfg) const {
		auto iter = m_creators.find(m_task_id_parser->parse(cfg));
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator with received type not found");
		}
		return (iter->second)->create(cfg);
	}

	template <class Tdata, class Treport, class Ttask_id>
	inline engine::Creator<engine::Task<Treport> *(const Tdata&)> *ServerFactory<Tdata, Treport, Ttask_id>::clone() const {
		return new ServerFactory<Tdata, Treport, Ttask_id>(*this);
	}
}
#endif // SERVER_FACTORY_HPP