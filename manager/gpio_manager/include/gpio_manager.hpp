#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "gpi.hpp"
#include "gpio.hpp"
#include "gpio_create_request.hpp"
#include "gpio_provider.hpp"
#include "gpio_request.hpp"
#include "gpio_response.hpp"
#include "gpio_state_response.hpp"
#include "gpio_write_request.hpp"
#include "gpo.hpp"
#include "manager.hpp"
#include "provider.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioManager: public mcu_app::Manager<GpioRequest<Tgpio_id>, GpioResponse *> {
	public:
		GpioManager(mcu_app::Provider *provider);
		GpioManager(const GpioManager& other) = delete;
		GpioManager& operator=(const GpioManager&) = delete;

		GpioResponse *run(const GpioRequest<Tgpio_id>& request) override;
	private:
		GpioProvider<Tgpio_id> *m_provider;
		std::map<Tgpio_id, std::unique_ptr<Gpio>> m_gpios;
		
		void create_gpio(const GpioRequest<Tgpio_id>& request);
		void delete_gpio(const GpioRequest<Tgpio_id>& request);
		Gpio::State read_gpio(const GpioRequest<Tgpio_id>& request) const;
		void write_gpio(const GpioRequest<Tgpio_id>& request);
	};
	
	template <typename Tgpio_id>
	inline GpioManager<Tgpio_id>::GpioManager(mcu_app::Provider *provider) {
		m_provider = dynamic_cast<GpioProvider<Tgpio_id> *>(provider);
		if (!m_provider) {
			throw std::invalid_argument("invalid provider received");
		}
	}

	template <typename Tgpio_id>
	inline GpioResponse *GpioManager<Tgpio_id>::run(const GpioRequest<Tgpio_id>& request) {
		switch (request.operation()) {
		case GpioRequest<Tgpio_id>::Operation::CREATE:
			create_gpio(request);
			return new GpioResponse(GpioResponse::Type::REGULAR, GpioResponse::Result::SUCCESS);
		case GpioRequest<Tgpio_id>::Operation::READ:
			return new GpioStateResponse(GpioResponse::Result::SUCCESS, read_gpio(request));
		case GpioRequest<Tgpio_id>::Operation::WRITE:
			write_gpio(request);
			return new GpioResponse(GpioResponse::Type::REGULAR, GpioResponse::Result::SUCCESS);
		case GpioRequest<Tgpio_id>::Operation::DELETE:
			delete_gpio(request);
			return new GpioResponse(GpioResponse::Type::REGULAR, GpioResponse::Result::SUCCESS);
		default:
			throw std::runtime_error("unsupported operation");
		}
	}

	template <typename Tgpio_id>
	void GpioManager<Tgpio_id>::create_gpio(const GpioRequest<Tgpio_id>& request) {
		const auto& create_request = dynamic_cast<const GpioCreateRequest<Tgpio_id>&>(request);
		m_gpios.insert(
			{
				create_request.id(),
				std::unique_ptr<Gpio>(m_provider->create_gpio(create_request.id(), create_request.direction()))
			}
		);
	}

	template <typename Tgpio_id>
	void GpioManager<Tgpio_id>::delete_gpio(const GpioRequest<Tgpio_id>& request) {
		const auto iter = m_gpios.find(request.id());
		if (m_gpios.end() == iter) {
			throw std::runtime_error("gpio with received ID is not created");
		}
		m_gpios.erase(iter);
	}

	template <typename Tgpio_id>
	Gpio::State GpioManager<Tgpio_id>::read_gpio(const GpioRequest<Tgpio_id>& request) const {
		const auto iter = m_gpios.find(request.id());
		if (m_gpios.end() == iter) {
			throw std::runtime_error("gpio with received ID is not created");
		}
		const auto& gpio = *(iter->second);
		switch (gpio.direction()) {
		case Gpio::Direction::IN:
			return Gpio::cast<Gpi>(gpio).state();
		case Gpio::Direction::OUT:
			return Gpio::cast<Gpo>(gpio).state();
		default:
			throw std::runtime_error("unsupported gpio type");
		}
	}
	
	template <typename Tgpio_id>
	void GpioManager<Tgpio_id>::write_gpio(const GpioRequest<Tgpio_id>& request) {
		const auto iter = m_gpios.find(request.id());
		if (m_gpios.end() == iter) {
			throw std::runtime_error("gpio with received ID is not created");
		}
		const auto& write_request = dynamic_cast<const GpioWriteRequest<Tgpio_id>&>(request);
		Gpio::cast<Gpo&>(*(iter->second)).set_state(write_request.state());
	}
}

#endif // GPIO_MANAGER_HPP