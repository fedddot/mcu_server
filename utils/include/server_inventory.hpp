#ifndef	SERVER_INVENTORY_HPP
#define	SERVER_INVENTORY_HPP

#include <map>
#include <stdexcept>

#include "inventory.hpp"

namespace server_utl {
	template <class Tkey, class Titem>
	class ServerInventory: public server::Inventory<Tkey, Titem> {
	public:
		ServerInventory() = default;
		ServerInventory(const ServerInventory& other) = delete;
		ServerInventory& operator=(const ServerInventory& other) = delete;

		virtual void put(const Tkey& key, Titem *item) override;
		virtual Titem *pull(const Tkey& key) override;
		virtual Titem *access(const Tkey& key) const override;
		virtual bool contains(const Tkey& key) const override;
	private:
		std::map<Tkey, Titem *> m_items;
	};

	template <class Tkey, class Titem>
	inline void ServerInventory<Tkey, Titem>::put(const Tkey& key, Titem *item) {
		if (m_items.end() != m_items.find(key)) {
			throw std::invalid_argument("received key is already registered");
		}
		if (!item) {
			throw std::invalid_argument("invalid item ptr received");
		}
		m_items.insert({ key, item });
	}

	template <class Tkey, class Titem>
	inline Titem *ServerInventory<Tkey, Titem>::pull(const Tkey& key) {
		auto iter = m_items.find(key);
		if (m_items.end() == iter) {
			throw std::invalid_argument("received key is not registered");
		}
		auto *result = iter->second;
		m_items.erase(iter);
		return result;
	}

	template <class Tkey, class Titem>
	inline Titem *ServerInventory<Tkey, Titem>::access(const Tkey& key) const {
		auto iter = m_items.find(key);
		if (m_items.end() == iter) {
			throw std::invalid_argument("received key is not registered");
		}
		return iter->second;
	}

	template <class Tkey, class Titem>
	inline bool ServerInventory<Tkey, Titem>::contains(const Tkey& key) const {
		return m_items.end() != m_items.find(key);
	}
}
#endif // SERVER_INVENTORY_HPP