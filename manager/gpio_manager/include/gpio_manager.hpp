#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include <map>
#include <stdexcept>

#include "gpio.hpp"
#include "gpio_provider.hpp"
#include "gpio_request.hpp"
#include "gpio_response.hpp"
#include "gpio_state_response.hpp"
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
		std::map<Tgpio_id, Gpio *> m_gpios;
		
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
			write_gpio(request.id(), request.state());
			return new GpioResponse(GpioResponse::Type::REGULAR, GpioResponse::Result::SUCCESS);
		case GpioRequest<Tgpio_id>::Operation::DELETE:
			delete_gpio(request.id());
			return new GpioResponse(GpioResponse::Type::REGULAR, GpioResponse::Result::SUCCESS);
		default:
			throw std::runtime_error("unsupported operation");
		}
	}
}

#endif // GPIO_MANAGER_HPP