#ifndef	GPIO_RESPONSE_WRAPPER_HPP
#define	GPIO_RESPONSE_WRAPPER_HPP

#include <memory>
#include <stdexcept>

#include "gpio_response.hpp"

namespace manager {
	class GpioResponseWrapper {
	public:
		GpioResponseWrapper(GpioResponse *response);
		GpioResponseWrapper(const GpioResponseWrapper&) = default;
		GpioResponseWrapper& operator=(const GpioResponseWrapper&) = default;
		virtual ~GpioResponseWrapper() noexcept = default;
		
		template <typename T>
		const T& get() const;
	private:
		std::shared_ptr<GpioResponse> m_response;
	};

	inline GpioResponseWrapper::GpioResponseWrapper(GpioResponse *response): m_response(response) {
		if (!m_response) {
			throw std::invalid_argument("invalid response pointer received");
		}
	}

	template <typename T>
	inline const T& GpioResponseWrapper::get() const {
		return dynamic_cast<const T&>(*m_response);
	}
}

#endif // GPIO_RESPONSE_WRAPPER_HPP