#ifndef	INVENTORY_MANAGER_HPP
#define	INVENTORY_MANAGER_HPP

#include <functional>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "inventory.hpp"
#include "manager.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"

namespace manager {
	template <typename Titem>
	class InventoryManager: public server::Manager {
	public:
		using ItemCreator = std::function<Titem *(const server::Data&)>;
		using ItemReader = std::function<server::Object(const Titem&)>;
		using ItemWriter = std::function<void(Titem *, const server::Data&)>;
		InventoryManager(
			Inventory<server::ResourceId, Titem> *inventory,
			const ItemCreator& item_creator,
			const ItemReader& item_reader,
			const ItemWriter& item_writer
		);
		InventoryManager(const InventoryManager& other) = delete;
		InventoryManager& operator=(const InventoryManager&) = delete;

		void create_resource(const server::ResourceId& id, const server::Data& create_cfg) override;
		server::Object read_resource(const server::Path& route) const override;
		server::Object read_all_resources() const override;
		void update_resource(const server::Path& route, const server::Data& update_cfg) override;
		void delete_resource(const server::Path& route) override;
		bool contains(const server::Path& route) const override;
	private:
		Inventory<server::ResourceId, Titem> *m_inventory;
		ItemCreator m_item_creator;
		ItemReader m_item_reader;
		ItemWriter m_item_writer;

		static server::ResourceId retrieve_resource_id_from_path(const server::Path& path);
	};
	
	template <typename Titem>
	inline InventoryManager<Titem>::InventoryManager(
			Inventory<server::ResourceId, Titem> *inventory,
			const ItemCreator& item_creator,
			const ItemReader& item_reader,
			const ItemWriter& item_writer
	): m_inventory(inventory), m_item_creator(item_creator), m_item_reader(item_reader), m_item_writer(item_writer) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
		if (!m_item_creator || !m_item_reader || !m_item_writer) {
			throw std::invalid_argument("invalid crw actions received");
		}
	}
	
	template <typename Titem>
	inline void InventoryManager<Titem>::create_resource(const server::ResourceId& id, const server::Data& create_cfg) {
		using namespace server;
		if (m_inventory->contains(id)) {
			throw ServerException(ResponseCode::BAD_REQUEST, "inventory item with id=" + id + " already registered");
		}
		m_inventory->add(id, m_item_creator(create_cfg));
	}
	
	template <typename Titem>
	inline server::Object InventoryManager<Titem>::read_resource(const server::Path& route) const {
		using namespace server;
		const auto id(retrieve_resource_id_from_path(route));
		if (!(m_inventory->contains(id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "inventory item with id=" + id + " is not registered");
		}
		return m_item_reader(m_inventory->access(id));
	}

	template <typename Titem>
	inline server::Object InventoryManager<Titem>::read_all_resources() const {
		using namespace server;
		Object items_data;
		for (const auto& id: m_inventory->ids()) {
			items_data.add(id, read_resource({id}));
		}
		return items_data;
	}

	template <typename Titem>
	inline void InventoryManager<Titem>::update_resource(const server::Path& route, const server::Data& update_cfg) {
		using namespace server;
		const auto id(retrieve_resource_id_from_path(route));
		if (!(m_inventory->contains(id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "inventory item with id=" + id + " is not registered");
		}
		auto& item_ref(m_inventory->access(id));
		m_item_writer(&item_ref, update_cfg);
	}

	template <typename Titem>
	inline void InventoryManager<Titem>::delete_resource(const server::Path& route) {
		using namespace server;
		const auto id(retrieve_resource_id_from_path(route));
		if (!(m_inventory->contains(id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "inventory item with id=" + id + " is not registered");
		}
		std::unique_ptr<Titem> item_ptr(m_inventory->remove(id));
		item_ptr = nullptr;
	}

	template <typename Titem>
	inline bool InventoryManager<Titem>::contains(const server::Path& route) const {
		using namespace server;
		return m_inventory->contains(retrieve_resource_id_from_path(route));
	}

	template <typename Titem>
	inline server::ResourceId InventoryManager<Titem>::retrieve_resource_id_from_path(const server::Path& route) {
		using namespace server;
		if (1UL != route.size()) {
			throw ServerException(ResponseCode::BAD_REQUEST, "bad route received (expected route length is 1, received " + std::to_string(route.size()) + ")");
		}
		return route[0];
	}
}

#endif // INVENTORY_MANAGER_HPP