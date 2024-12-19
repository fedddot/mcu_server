#ifndef	IN_MEMORY_INVENTORY_HPP
#define	IN_MEMORY_INVENTORY_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <vector>

#include "inventory.hpp"

namespace manager {
	template <typename Tid, typename Tstored>
	class InMemoryInventory: public Inventory<Tid, Tstored> {
	public:
		InMemoryInventory() = default;
		InMemoryInventory(const InMemoryInventory&) = delete;
		InMemoryInventory& operator=(const InMemoryInventory&) = delete;
		
		void add(const Tid& id, Tstored *item) override;
		Tstored *remove(const Tid& id) override;
		bool contains(const Tid& id) const override;
		Tstored& access(const Tid& id) override;
		const Tstored& access(const Tid& id) const override;
		const std::vector<Tid> ids() const override;
	private:
		std::map<Tid, Tstored *> m_items;
	};

	template <typename Tid, typename Tstored>
	inline void InMemoryInventory<Tid, Tstored>::add(const Tid& id, Tstored *item) {
		if (m_items.end() != m_items.find(id)) {
			throw std::invalid_argument("received id already exists");
		}
		if (!item) {
			throw std::invalid_argument("invalid item ptr received");
		}
		m_items.insert({id, item});
	}

	template <typename Tid, typename Tstored>
	inline Tstored *InMemoryInventory<Tid, Tstored>::remove(const Tid& id) {
		auto iter = m_items.find(id);
		if (m_items.end() == iter) {
			throw std::invalid_argument("item with received id doesn't exist");
		}
		auto item_ptr = iter->second;
		m_items.erase(iter);
		return item_ptr;
	}

	template <typename Tid, typename Tstored>
	inline bool InMemoryInventory<Tid, Tstored>::contains(const Tid& id) const {
		return m_items.end() != m_items.find(id);
	}

	template <typename Tid, typename Tstored>
	inline Tstored& InMemoryInventory<Tid, Tstored>::access(const Tid& id) {
		auto iter = m_items.find(id);
		if (m_items.end() == iter) {
			throw std::invalid_argument("item with received id doesn't exist");
		}
		return std::ref(*(iter->second));
	}

	template <typename Tid, typename Tstored>
	inline const Tstored& InMemoryInventory<Tid, Tstored>::access(const Tid& id) const {
		auto iter = m_items.find(id);
		if (m_items.end() == iter) {
			throw std::invalid_argument("item with received id doesn't exist");
		}
		return std::ref(*(iter->second));
	}

	template <typename Tid, typename Tstored>
	inline const std::vector<Tid> InMemoryInventory<Tid, Tstored>::ids() const {
		std::vector<Tid> items_ids;
		items_ids.reserve(m_items.size());
		for (const auto& [id, item_ptr]: m_items) {
			(void)item_ptr;
			items_ids.push_back(id);
		}
		return items_ids;
	}
}

#endif // IN_MEMORY_INVENTORY_HPP