#ifndef	GPIO_MANAGER_HPP
#define	GPIO_MANAGER_HPP

#include "gpio_data.hpp"
#include "gpio_provider.hpp"
#include "gpio_request.hpp"
#include "gpio_response.hpp"
#include "manager.hpp"
#include "provider.hpp"
#include <map>
#include <stdexcept>

namespace manager {
	class GpioManager: public mcu_app::Manager<GpioRequest, GpioResponse> {
	public:
		GpioManager(mcu_app::Provider *provider);
		GpioManager(const GpioManager& other) = delete;
		GpioManager& operator=(const GpioManager&) = delete;

		GpioResponse run(const GpioRequest& request) override;
	private:
		GpioProvider *m_provider;
		std::map<GpioData::GpioId, Gpio *> m_gpios;
		
		void create_gpio(const GpioData& data);
		void delete_gpio(const GpioData& data);
		Gpio::State read_gpio(const GpioData& data) const;
		void write_gpio(const GpioData& data);
	};
	
	inline GpioManager::GpioManager(mcu_app::Provider *provider) {
		m_provider = dynamic_cast<GpioProvider *>(provider);
		if (!m_provider) {
			throw std::invalid_argument("invalid provider received");
		}
	}

	inline GpioResponse run(const GpioRequest& request) {
		switch (request.operation()) {
			case GpioRequest::GpioOperation::CREATE:
				
		}
	}
}

#endif // GPIO_MANAGER_HPP