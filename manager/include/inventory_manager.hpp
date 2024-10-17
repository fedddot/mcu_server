#ifndef	INVENTORY_MANAGER_HPP
#define	INVENTORY_MANAGER_HPP

#include <exception>
#include <functional>
#include <stdexcept>
#include <string>

#include "inventory.hpp"
#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace manager {
	template <typename Tid, typename Tstored>
	class InventoryManager: public server::Resource {
	public:
		using InventoryRequestHandler = std::function<server::Response(Inventory<Tid, Tstored> *inventory, const server::Request&)>;
		
		InventoryManager(Inventory<Tid, Tstored> *inventory, const InventoryRequestHandler& inventory_request_handler);
		InventoryManager(const InventoryManager& other) = default;
		InventoryManager& operator=(const InventoryManager&) = delete;
		
		server::Response run_request(const server::Request& request) const override;
		server::Resource *clone() const override;
	private:
		mutable Inventory<Tid, Tstored> *m_inventory;
		InventoryRequestHandler m_inventory_request_handler;

		using ResponseCode = typename server::Response::ResponseCode;
		server::Response report_failure(const ResponseCode& code, const std::string& what) const;
	};

	template <typename Tid, typename Tstored>
	inline InventoryManager<Tid, Tstored>::InventoryManager(Inventory<Tid, Tstored> *inventory, const InventoryRequestHandler& inventory_request_handler): m_inventory(inventory), m_inventory_request_handler(inventory_request_handler) {
		if (!m_inventory || !m_inventory_request_handler) {
			throw std::invalid_argument("invalid arguments received");
		}
	}

	template <typename Tid, typename Tstored>
	inline server::Response InventoryManager<Tid, Tstored>::run_request(const server::Request& request) const {
		try {
			return m_inventory_request_handler(request);
		} catch (const std::exception& e) {
			return report_failure(ResponseCode::UNSPECIFIED, std::string(e.what()));
		}
	}

	template <typename Tid, typename Tstored>
	inline server::Resource *InventoryManager<Tid, Tstored>::clone() const {
		return new InventoryManager(*this);
	}

	template <typename Tid, typename Tstored>
	inline server::Response InventoryManager<Tid, Tstored>::report_failure(const ResponseCode& code, const std::string& what) const {
		using namespace server;
		using Body = typename Response::Body;
		Body failure_body;
		failure_body.add("what", String(what));
		return Response(ResponseCode::UNSPECIFIED, failure_body);
	}
}

#endif // INVENTORY_MANAGER_HPP