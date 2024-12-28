#ifndef	GPIO_IPC_FACTORY_HPP
#define	GPIO_IPC_FACTORY_HPP

#include <stdexcept>

#include "cpprest/http_msg.h"
#include "data.hpp"
#include "gpio_app_types.hpp"
#include "gpio_request.hpp"
#include "integer.hpp"
#include "ipc_connection.hpp"
#include "object.hpp"
#include "string.hpp"

namespace gpio_app {
	template <typename Tgpio_id>
	class GpioIpcFactory {
	public:
		using GpioIpcConnection = mcu_app::IpcConnection<manager::GpioRequest<Tgpio_id>, manager::GpioResponseWrapper>;
		GpioIpcFactory() = default;
		GpioIpcFactory(const GpioIpcFactory&) = default;
		GpioIpcFactory& operator=(const GpioIpcFactory&) = default;
		virtual ~GpioIpcFactory() noexcept = default;

		GpioIpcConnection *operator()(const IpcConfig& config) const;
	private:
		static GpioIpcConnection *create_http_connection(const data::Object& config);
	};

	template <typename Tgpio_id>
	typename GpioIpcFactory<Tgpio_id>::GpioIpcConnection *GpioIpcFactory<Tgpio_id>::operator()(const IpcConfig& config) const {
		using namespace data;
		const auto type = Data::cast<String>(config.access("type")).get();
		const auto cfg = Data::cast<Object>(config.access("config"));

		if ("http" == type) {
			return create_http_connection(cfg);
		}
		throw std::invalid_argument("unsupported connection type");
	}

	template <typename Tgpio_id>
	typename GpioIpcFactory<Tgpio_id>::GpioIpcConnection *GpioIpcFactory<Tgpio_id>::create_http_connection(const data::Object& config) {
		using namespace data;
		const auto url = Data::cast<String>(config.access("url")).get();
		const auto polling_timeout_s = Data::cast<Integer>(config.access("polling_timeout_s")).get();
		const auto response_timeout_s = Data::cast<Integer>(config.access("response_timeout_s")).get();
        auto request_transformer = [](const web::http::http_request& request)-> manager::GpioRequest<Tgpio_id> {
                return request;
            },
            [](const web::http::http_response& response) {

                return response;
		}
	}
}

#endif // GPIO_IPC_FACTORY_HPP