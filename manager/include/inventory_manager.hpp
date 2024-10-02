#ifndef	INVENTORY_MANAGER_HPP
#define	INVENTORY_MANAGER_HPP

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <map>

#include "array.hpp"
#include "data.hpp"
#include "object.hpp"
#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace server {
	template <typename Tstored>
	class InventoryManager: public Resource {
	public:
		using Creator = std::function<Tstored *(const Data&)>;
		using Duplicator = std::function<Tstored *(const Tstored&)>;
		using Reader = std::function<Object(const Tstored&)>;
		using Writer = std::function<void(Tstored *, const Data&)>;

		InventoryManager(const Creator& creator, const Duplicator& duplicator, const Reader& reader, const Writer& writer);
		InventoryManager(const InventoryManager& other);
		InventoryManager& operator=(const InventoryManager&) = delete;
		
		Response run_request(const Request& request) const override;
		Resource *clone() const override;
	private:
		Creator m_creator;
		Duplicator m_duplicator;
		Reader m_reader;
		Writer m_writer;

		using InstanceUniquePtr = std::unique_ptr<Tstored>;
		using Instances = std::map<std::string, InstanceUniquePtr>;
		mutable Instances m_instances;

		Response run_create(const Request& request) const;
		Response run_read(const Request& request) const;
		Response run_update(const Request& request) const;
		Response run_delete(const Request& request) const;

		using ResponseCode = typename Response::ResponseCode;
		Response report_failure(const ResponseCode& code, const std::string& what) const;
	};

	template <typename Tstored>
	inline InventoryManager<Tstored>::InventoryManager(const Creator& creator, const Duplicator& duplicator, const Reader& reader, const Writer& writer): m_creator(creator), m_duplicator(duplicator), m_reader(reader), m_writer(writer) {
		if (!m_creator || !m_duplicator || !m_reader || !m_writer) {
			throw std::invalid_argument("invalid action received");
		}
	}

	template <typename Tstored>
	inline InventoryManager<Tstored>::InventoryManager(const InventoryManager& other): m_creator(other.m_creator), m_duplicator(other.m_duplicator), m_reader(other.m_reader), m_writer(other.m_writer) {
		for (const auto& [id, instance_ptr]: other.m_instances) {
			m_instances.insert({id, InstanceUniquePtr(m_duplicator(*instance_ptr))});
		}
	}

	template <typename Tstored>
	inline Response InventoryManager<Tstored>::run_request(const Request& request) const {
		try {
			using Method = typename Request::Method;
			switch (request.method()) {
			case Method::CREATE:
				return run_create(request);
			case Method::READ:
				return run_read(request);
			case Method::UPDATE:
				return run_update(request);
			case Method::DELETE:
				return run_delete(request);
			default:
				return report_failure(ResponseCode::BAD_REQUEST, "unsupported method request received");
			}
		} catch (const std::exception& e) {
			return report_failure(ResponseCode::UNSPECIFIED, std::string(e.what()));
		}
	}

	template <typename Tstored>
	inline Resource *InventoryManager<Tstored>::clone() const {
		return new InventoryManager(*this);
	}

	template <typename Tstored>
	inline Response InventoryManager<Tstored>::run_create(const Request& request) const {
		using Body = typename Response::Body;
		if (!request.path().empty()) {
			return report_failure(ResponseCode::NOT_FOUND, "wrong create request path");
		}
		auto id(Data::cast<String>(request.body().access("id")).get());
		if (m_instances.end() != m_instances.find(id)) {
			return report_failure(ResponseCode::BAD_REQUEST, "component with id = " + id + " already exists");
		}
		m_instances.insert({id, std::unique_ptr<Tstored>(m_creator(request.body()))});
		return Response(ResponseCode::OK, Body());
	}

	template <typename Tstored>
	inline Response InventoryManager<Tstored>::run_read(const Request& request) const {
		auto read_instance = [this](const std::string& id) {
			auto iter = m_instances.find(id);
			if (m_instances.end() == iter) {
				return report_failure(ResponseCode::NOT_FOUND, "resource " + id + " is not registered");
			}
			auto instance_data(m_reader(*(iter->second)));
			instance_data.add("id", String(iter->first));
			return Response(ResponseCode::OK, instance_data);
		};

		auto read_all_instances = [this](void) {
			using Body = typename Response::Body;
			Array members;
			for (const auto& [id, instance_ptr]: m_instances) {
				auto member_data(m_reader(*instance_ptr));
				member_data.add("id", String(id));
				members.push_back(member_data);
			}
			Body body;
			body.add("members", members);
			return Response(ResponseCode::OK, body);
		};

		if (1UL < request.path().size()) {
			return report_failure(ResponseCode::NOT_FOUND, std::string("path not found"));
		}

		if (request.path().empty()) {
			return read_all_instances();
		}

		auto id(request.path()[0]);
		return read_instance(id);
	}

	template <typename Tstored>
	inline Response InventoryManager<Tstored>::run_update(const Request& request) const {
		if (1UL != request.path().size()) {
			return report_failure(ResponseCode::NOT_FOUND, std::string("path not found"));
		}
		auto id(request.path()[0]);
		auto iter = m_instances.find(id);
		if (m_instances.end() == iter) {
			return report_failure(ResponseCode::NOT_FOUND, std::string("path not found"));
		}
		m_writer((iter->second).get(), request.body());
		return Response(ResponseCode::OK, Response::Body());
	}

	template <typename Tstored>
	inline Response InventoryManager<Tstored>::run_delete(const Request& request) const {
		if (1UL != request.path().size()) {
			return report_failure(ResponseCode::NOT_FOUND, std::string("path not found"));
		}
		auto id(request.path()[0]);
		auto iter = m_instances.find(id);
		if (m_instances.end() == iter) {
			return report_failure(ResponseCode::NOT_FOUND, std::string("path not found"));
		}
		m_instances.erase(iter);
		return Response(ResponseCode::OK, Response::Body());
	}

	template <typename Tstored>
	inline Response InventoryManager<Tstored>::report_failure(const ResponseCode& code, const std::string& what) const {
		using Body = typename Response::Body;
		Body failure_body;
		failure_body.add("what", String(what));
		return Response(ResponseCode::UNSPECIFIED, failure_body);
	}
}

#endif // INVENTORY_MANAGER_HPP