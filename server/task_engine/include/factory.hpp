#ifndef	FACTORY_HPP
#define	FACTORY_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "creator.hpp"
#include "parser.hpp"

namespace server {
	template <typename Tctor_id, typename Tprod, typename Tdata>
	class Factory: public engine::Creator<Tprod(const Tdata&)> {
	public:
		using CtorIdParser = engine::Parser<Tctor_id(const Tdata&)>;
		using ItemCreator = engine::Creator<Tprod(const Tdata&)>;
		
		Factory(const CtorIdParser& task_id_parser);
		Factory(const Factory& other);
		Factory& operator=(const Factory& other) = delete;

		Tprod create(const Tdata& cfg) const override;
		engine::Creator<Tprod(const Tdata&)> *clone() const override;

		void register_creator(const Tctor_id& id, const ItemCreator& creator);
	private:
		std::unique_ptr<CtorIdParser> m_task_id_parser;
		std::map<Tctor_id, std::unique_ptr<ItemCreator>> m_creators;
	};

	template <typename Tctor_id, typename Tprod, typename Tdata>
	inline Factory<Tctor_id, Tprod, Tdata>::Factory(const CtorIdParser& task_id_parser): m_task_id_parser(task_id_parser.clone()) {

	}

	template <typename Tctor_id, typename Tprod, typename Tdata>
	inline Factory<Tctor_id, Tprod, Tdata>::Factory(const Factory& other): m_task_id_parser(other.m_task_id_parser->clone()) {
		for (auto& item: other.m_creators) {
			m_creators.insert(
				{item.first, std::unique_ptr<ItemCreator>(item.second->clone())}
			);
		}
	}

	template <typename Tctor_id, typename Tprod, typename Tdata>
	inline void Factory<Tctor_id, Tprod, Tdata>::register_creator(const Tctor_id& id, const ItemCreator& creator) {
		m_creators[id] = std::unique_ptr<ItemCreator>(creator.clone());
	}

	template <typename Tctor_id, typename Tprod, typename Tdata>
	inline Tprod Factory<Tctor_id, Tprod, Tdata>::create(const Tdata& cfg) const {
		auto iter = m_creators.find(m_task_id_parser->parse(cfg));
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator with received type not found");
		}
		return (iter->second)->create(cfg);
	}

	template <typename Tctor_id, typename Tprod, typename Tdata>
	inline engine::Creator<Tprod(const Tdata&)> *Factory<Tctor_id, Tprod, Tdata>::clone() const {
		return new Factory<Tctor_id, Tprod, Tdata>(*this);
	}
}
#endif // FACTORY_HPP