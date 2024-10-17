#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include <functional>
#include <memory>
#include <string>

#include "gpio.hpp"
#include "inventory.hpp"
#include "inventory_manager.hpp"
#include "request.hpp"
#include "resource.hpp"
#include "response.hpp"
#include "string.hpp"

namespace manager {
	template <typename Tid>
	class GpioManager: public InventoryManager<Tid, Gpio> {
	public:
		using GpioCreator = std::function<Gpio *(const server::Request::Body&)>;
		using GpioIdFromBodyRetriever = std::function<Tid(const server::Request::Body&)>;
		using GpioIdFromPathRetriever = std::function<Tid(const server::Request::Path&)>;
		using GpioReader = std::function<server::Response::Body(const Gpio&)>;
		using GpioWriter = std::function<void(Gpio *, const server::Request::Body&)>;

		GpioManager(
			Inventory<Tid, Gpio> *gpio_inventory,
			const GpioCreator& gpio_creator,
			const GpioIdFromBodyRetriever& gpio_id_from_body_retriever,
			const GpioIdFromPathRetriever& gpio_id_from_path_retriever,
			const GpioReader& gpio_reader,
			const GpioWriter& gpio_writer
		);
		GpioManager(const GpioManager& other) = default;
		GpioManager& operator=(const GpioManager&) = delete;
		server::Resource *clone() const override;
	private:
		GpioCreator m_gpio_creator;
		GpioIdFromBodyRetriever m_gpio_id_from_body_retriever;
		GpioIdFromPathRetriever m_gpio_id_from_path_retriever;
		GpioReader m_gpio_reader;
		GpioWriter m_gpio_writer;

		server::Response handle_request(Inventory<Tid, Gpio> *inventory, const server::Request& request) const;
		server::Response create_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const;
		server::Response read_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const;
		server::Response update_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const;
		server::Response delete_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const;
		
		server::Response report_failure(const server::Response::ResponseCode& code, const std::string& what) const;
	};

	template <typename Tid>
	inline GpioManager<Tid>::GpioManager(
		Inventory<Tid, Gpio> *gpio_inventory,
		const GpioCreator& gpio_creator,
		const GpioIdFromBodyRetriever& gpio_id_from_body_retriever,
		const GpioIdFromPathRetriever& gpio_id_from_path_retriever,
		const GpioReader& gpio_reader,
		const GpioWriter& gpio_writer
	): InventoryManager<Tid, Gpio>(
		gpio_inventory,
		[this](Inventory<Tid, Gpio> *inventory, const server::Request& request) {
			return handle_request(inventory, request);
		}
	), m_gpio_creator(gpio_creator), m_gpio_id_from_body_retriever(gpio_id_from_body_retriever), m_gpio_id_from_path_retriever(gpio_id_from_path_retriever), m_gpio_reader(gpio_reader), m_gpio_writer(gpio_writer) {

	}

	template <typename Tid>
	inline server::Response GpioManager<Tid>::create_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		if (!request.path().empty()) {
			return report_failure(ResponseCode::BAD_REQUEST, "create gpio handler requires an empty path relatively to the manager");
		}
		auto id(m_gpio_id_from_body_retriever(request.body()));
		if (inventory->contains(id)) {
			return report_failure(ResponseCode::BAD_REQUEST, "gpio with specified id already exists");
		}
		inventory->add(id, m_gpio_creator(request.body()));
		return server::Response(ResponseCode::OK, server::Response::Body());
	}

	template <typename Tid>
	inline server::Response GpioManager<Tid>::read_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		const auto gpio_id(m_gpio_id_from_path_retriever(request.path()));
		if (!(inventory->contains(gpio_id))) {
			return report_failure(ResponseCode::NOT_FOUND, "gpio with specified id doesn't exist");
		}
		auto body(m_gpio_reader(inventory->access(gpio_id)));
		return server::Response(ResponseCode::OK, body);
	}

	template <typename Tid>
	inline server::Response GpioManager<Tid>::update_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		const auto gpio_id(m_gpio_id_from_path_retriever(request.path()));
		if (!(inventory->contains(gpio_id))) {
			return report_failure(ResponseCode::NOT_FOUND, "gpio with specified id doesn't exist");
		}
		m_gpio_writer(&(inventory->access(gpio_id)), request.body());
		return server::Response(ResponseCode::OK, server::Response::Body());
	}

	template <typename Tid>
	inline server::Response GpioManager<Tid>::delete_gpio(Inventory<Tid, Gpio> *inventory, const server::Request& request) const {
		using ResponseCode = typename server::Response::ResponseCode;
		const auto gpio_id(m_gpio_id_from_path_retriever(request.path()));
		if (!(inventory->contains(gpio_id))) {
			return report_failure(ResponseCode::NOT_FOUND, "gpio with specified id doesn't exist");
		}
		std::unique_ptr<Gpio> removed_gpio(inventory->remove(gpio_id));
		removed_gpio = nullptr;
		return server::Response(ResponseCode::OK, server::Response::Body());
	}

	template <typename Tid>
	inline server::Response GpioManager<Tid>::handle_request(Inventory<Tid, Gpio> *inventory, const server::Request& request) const {
		using Method = typename server::Request::Method;
		using ResponseCode = typename server::Response::ResponseCode;

		switch (request.method()) {
		case Method::CREATE:
			return create_gpio(inventory, request);
		case Method::READ:
			return read_gpio(inventory, request);
		case Method::UPDATE:
			return update_gpio(inventory, request);
		case Method::DELETE:
			return delete_gpio(inventory, request);
		default:
			return report_failure(ResponseCode::BAD_REQUEST, "unsupported method");
		}
	}

	template <typename Tid>
	inline server::Response GpioManager<Tid>::report_failure(const server::Response::ResponseCode& code, const std::string& what) const {
		server::Response::Body body;
		body.add("what", server::String(what));
		return server::Response(code, body);
	}
	
	template <typename Tid>
	inline server::Resource *GpioManager<Tid>::clone() const {
		return new GpioManager(*this);
	}
}

#endif // GPIO_MANAGER_HPP