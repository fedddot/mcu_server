#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

#include "gpi.hpp"
#include "gpio.hpp"
#include "gpo.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "manager.hpp"
#include "object.hpp"
#include "server_exception.hpp"
#include "server_types.hpp"
#include "string.hpp"

namespace manager {
	class GpioManager: public server::Manager {
	public:
		using GpioCreator = std::function<Gpio *(const server::Body&)>;
		GpioManager(Inventory<server::ResourceId, Gpio> *gpio_inventory, const GpioCreator& gpio_creator);
		GpioManager(const GpioManager& other) = delete;
		GpioManager& operator=(const GpioManager&) = delete;

		void create_resource(const server::Body& create_request_body) override;
		server::Body read_resource(const server::Path& route) const override;
		server::Body read_all_resources() const override;
		void update_resource(const server::Path& route, const server::Body& update_request_body) override;
		void delete_resource(const server::Path& route) override;
		bool contains(const server::Path& route) const override;
	private:
		Inventory<server::ResourceId, Gpio> *m_gpio_inventory;
		GpioCreator m_gpio_creator;

		static server::ResourceId get_id_from_route(const server::Path& route);
	};
	
	inline GpioManager::GpioManager(Inventory<server::ResourceId, Gpio> *gpio_inventory, const GpioCreator& gpio_creator): m_gpio_inventory(gpio_inventory), m_gpio_creator(gpio_creator) {
		if (!m_gpio_inventory || !m_gpio_creator) {
			throw std::invalid_argument("invalid arguments received");
		}
	}
	
	inline void GpioManager::create_resource(const server::Body& create_request_body) {
		using namespace server;
		auto id(static_cast<ResourceId>(Data::cast<String>(create_request_body.access("id")).get()));
		if (m_gpio_inventory->contains(id)) {
			throw ServerException(ResponseCode::BAD_REQUEST, "gpio with id " + id + " already exists");
		}
		m_gpio_inventory->add(id, m_gpio_creator(create_request_body));
	}
	
	inline server::Body GpioManager::read_resource(const server::Path& route) const {
		using namespace server;
		const auto gpio_id(get_id_from_route(route));
		if (!(m_gpio_inventory->contains(gpio_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "gpio with specified id doesn't exist");
		}
		const auto& gpio(m_gpio_inventory->access(gpio_id));
		Body response_body;
		response_body.add("dir", Integer(static_cast<int>(gpio.direction())));
		switch (gpio.direction()) {
		case Gpio::Direction::IN:
			response_body.add("state", Integer(static_cast<int>(Gpio::cast<Gpi>(gpio).state())));
			break;
		case Gpio::Direction::OUT:
			response_body.add("state", Integer(static_cast<int>(Gpio::cast<Gpo>(gpio).state())));
			break;
		default:
			throw ServerException(ResponseCode::UNSPECIFIED, "unsupported gpio direction");
		}
		return response_body;
	}

	inline server::Body GpioManager::read_all_resources() const {
		using namespace server;
		Body response_body;
		for (const auto& id: m_gpio_inventory->ids()) {
			response_body.add(id, read_resource({id}));
		}
		return response_body;
	}

	inline void GpioManager::update_resource(const server::Path& route, const server::Body& update_body) {
		using namespace server;
		const auto gpio_id(get_id_from_route(route));
		if (!(m_gpio_inventory->contains(gpio_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "gpio with specified id doesn't exist");
		}
		auto& gpio(m_gpio_inventory->access(gpio_id));
		if (Gpio::Direction::OUT != gpio.direction()) {
			throw ServerException(ResponseCode::BAD_REQUEST, "specified gpio is not an output");
		}
		const auto state(static_cast<Gpio::State>(Data::cast<Integer>(Data::cast<Object>(update_body.access("config")).access("state")).get()));
		Gpio::cast<Gpo>(gpio).set_state(state);
	}
	
	inline void GpioManager::delete_resource(const server::Path& route) {
		using namespace server;
		const auto gpio_id(get_id_from_route(route));
		if (!(m_gpio_inventory->contains(gpio_id))) {
			throw ServerException(ResponseCode::NOT_FOUND, "gpio with specified id doesn't exist");
		}
		std::unique_ptr<Gpio> gpio_ptr(m_gpio_inventory->remove(gpio_id));
		gpio_ptr = nullptr;
	}

	inline bool GpioManager::contains(const server::Path& route) const {
		using namespace server;
		return m_gpio_inventory->contains(get_id_from_route(route));
	}

	inline server::ResourceId GpioManager::get_id_from_route(const server::Path& route) {
		using namespace server;
		if (1UL != route.size()) {
			throw ServerException(ResponseCode::BAD_REQUEST, "bad route received");
		}
		return route[0];
	}
}

#endif // GPIO_MANAGER_HPP