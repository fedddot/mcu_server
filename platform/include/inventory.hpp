#ifndef	INVENTORY_HPP
#define	INVENTORY_HPP

#include <map>
#include <stdexcept>

namespace mcu_platform {
	template <class Tkey, class Titem>
	class Inventory {
	public:
		Inventory() = default;
		Inventory(const Inventory& other) = delete;
		Inventory& operator=(const Inventory& other) = delete;
		virtual ~Inventory() noexcept = default;

		void put(const Tkey& key, Titem *item);
		Titem *pull(const Tkey& key);
		Titem *access(const Tkey& key) const;
		bool contains(const Tkey& key) const;
	private:
		std::map<Tkey, Titem *> m_items;
	};

	template <class Tkey, class Titem>
	inline void Inventory<Tkey, Titem>::put(const Tkey& key, Titem *item) {
		if (m_items.end() != m_items.find(key)) {
			throw std::invalid_argument("received key is already registered");
		}
		if (!item) {
			throw std::invalid_argument("invalid item ptr received");
		}
		m_items.insert({ key, item });
	}

	template <class Tkey, class Titem>
	inline Titem *Inventory<Tkey, Titem>::pull(const Tkey& key) {
		auto iter = m_items.find(key);
		if (m_items.end() == iter) {
			throw std::invalid_argument("received key is not registered");
		}
		auto *result = iter->second;
		m_items.erase(iter);
		return result;
	}

	template <class Tkey, class Titem>
	inline Titem *Inventory<Tkey, Titem>::access(const Tkey& key) const {
		auto iter = m_items.find(key);
		if (m_items.end() == iter) {
			throw std::invalid_argument("received key is not registered");
		}
		return iter->second;
	}

	template <class Tkey, class Titem>
	inline bool Inventory<Tkey, Titem>::contains(const Tkey& key) const {
		return m_items.end() != m_items.find(key);
	}
}
#endif // INVENTORY_HPP