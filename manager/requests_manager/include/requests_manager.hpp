#ifndef	REQUESTS_MANAGER_HPP
#define	REQUESTS_MANAGER_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "inventory.hpp"
#include "inventory_manager.hpp"
#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace manager {
	template <typename Tid>
	class RequestsManager: public InventoryManager<Tid, server::Request> {
	public:
		using RequestRetriever = std::function<server::Request(const server::Request::Body&)>;
		using RequestIdFromBodyRetriever = std::function<Tid(const server::Request::Body&)>;
		using RequestIdFromPathRetriever = std::function<Tid(const server::Request::Path&)>;
		using RequestsIdsRetriever = std::function<std::vector<Tid>(const server::Request::Body&)>;
		using RequestReader = std::function<server::Response::Body(const server::Request&)>;

		RequestsManager(
			Inventory<Tid, server::Request> *requests_inventory,
			server::Resource *vendor,
			const RequestRetriever& request_retriever,
			const RequestIdFromBodyRetriever& request_id_from_body_retriever,
			const RequestIdFromPathRetriever& request_id_from_path_retriever,
			const RequestsIdsRetriever& requests_ids_retriever,
			const RequestReader& request_reader
		);
		RequestsManager(const RequestsManager& other) = default;
		RequestsManager& operator=(const RequestsManager&) = delete;
		server::Resource *clone() const override;
	private:
		server::Resource *m_vendor;
		RequestRetriever m_request_retriever;
		RequestIdFromBodyRetriever m_request_id_from_body_retriever;
		RequestIdFromPathRetriever m_request_id_from_path_retriever;
		RequestsIdsRetriever m_requests_ids_retriever;
		RequestReader m_request_reader;

		server::Response handle_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const;
		server::Response create_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const;
		server::Response read_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const;
		server::Response update_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const;
		server::Response delete_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const;
		
		server::Response report_failure(const server::Response::ResponseCode& code, const std::string& what) const;
	};

	template <typename Tid>
	inline RequestsManager<Tid>::RequestsManager(
		Inventory<Tid, server::Request> *requests_inventory,
		server::Resource *vendor,
		const RequestRetriever& request_retriever,
		const RequestIdFromBodyRetriever& request_id_from_body_retriever,
		const RequestIdFromPathRetriever& request_id_from_path_retriever,
		const RequestsIdsRetriever& requests_ids_retriever,
		const RequestReader& request_reader
	): InventoryManager<Tid, server::Request>(
		requests_inventory,
		[this](Inventory<Tid, server::Request> *inventory, const server::Request& request) {
			return handle_request(inventory, request);
		}
	), m_vendor(vendor), m_request_retriever(request_retriever), m_request_id_from_body_retriever(request_id_from_body_retriever), m_request_id_from_path_retriever(request_id_from_path_retriever), m_requests_ids_retriever(requests_ids_retriever), m_request_reader(request_reader) {
		if (!m_vendor) {
			throw std::invalid_argument("invalid vendor ptr received");
		}
	}

	template <typename Tid>
	inline server::Response RequestsManager<Tid>::create_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		if (!request.path().empty()) {
			return report_failure(ResponseCode::BAD_REQUEST, "create request handler requires an empty path relatively to the manager");
		}
		auto id(m_request_id_from_body_retriever(request.body()));
		if (inventory->contains(id)) {
			return report_failure(ResponseCode::BAD_REQUEST, "request with specified id already exists");
		}
		inventory->add(id, new server::Request(m_request_retriever(request.body())));
		return server::Response(ResponseCode::OK, server::Response::Body());
	}

	template <typename Tid>
	inline server::Response RequestsManager<Tid>::read_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		const auto request_id(m_request_id_from_path_retriever(request.path()));
		if (!(inventory->contains(request_id))) {
			return report_failure(ResponseCode::NOT_FOUND, "request with specified id doesn't exist");
		}
		auto body(m_request_reader(inventory->access(request_id)));
		return server::Response(ResponseCode::OK, body);
	}

	template <typename Tid>
	inline server::Response RequestsManager<Tid>::update_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		if (!request.path().empty()) {
			return report_failure(ResponseCode::BAD_REQUEST, "update request handler requires an empty path relatively to the manager");
		}		
		const auto ids(m_requests_ids_retriever(request.body()));
		for (const auto& id: ids) {
			if (!(inventory->contains(id))) {
				return report_failure(ResponseCode::NOT_FOUND, "request with received id doesn't exist");
			}
			auto request_response(m_vendor->run_request(inventory->access(id)));
			if (ResponseCode::OK != request_response.code()) {
				return request_response;
			}
		}
		return server::Response(ResponseCode::OK, server::Response::Body());
	}

	template <typename Tid>
	inline server::Response RequestsManager<Tid>::delete_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		const auto request_id(m_request_id_from_path_retriever(request.path()));
		if (!(inventory->contains(request_id))) {
			return report_failure(ResponseCode::NOT_FOUND, "request with specified id doesn't exist");
		}
		std::unique_ptr<server::Request> removed_request(inventory->remove(request_id));
		removed_request = nullptr;
		return server::Response(ResponseCode::OK, server::Response::Body());
	}

	template <typename Tid>
	inline server::Response RequestsManager<Tid>::handle_request(Inventory<Tid, server::Request> *inventory, const server::Request& request) const {
		using Method = typename server::Request::Method;
		using ResponseCode = typename server::Response::ResponseCode;

		switch (request.method()) {
		case Method::CREATE:
			return create_request(inventory, request);
		case Method::READ:
			return read_request(inventory, request);
		case Method::UPDATE:
			return update_request(inventory, request);
		case Method::DELETE:
			return delete_request(inventory, request);
		default:
			return report_failure(ResponseCode::BAD_REQUEST, "unsupported method");
		}
	}

	template <typename Tid>
	inline server::Response RequestsManager<Tid>::report_failure(const server::Response::ResponseCode& code, const std::string& what) const {
		server::Response::Body body;
		body.add("what", server::String(what));
		return server::Response(code, body);
	}
	
	template <typename Tid>
	inline server::Resource *RequestsManager<Tid>::clone() const {
		return new RequestsManager(*this);
	}
}

#endif // REQUESTS_MANAGER_HPP