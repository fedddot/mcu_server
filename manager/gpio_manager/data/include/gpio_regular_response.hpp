#ifndef	GPIO_REGULAR_RESPONSE_HPP
#define	GPIO_REGULAR_RESPONSE_HPP

#include "gpio_response.hpp"

namespace manager {
	class GpioRegularResponse: public GpioResponse {
	public:
		GpioRegularResponse(const Result& result);
		GpioRegularResponse(const GpioRegularResponse&) = default;
		GpioRegularResponse& operator=(const GpioRegularResponse&) = default;
		Type type() const override;
		Result result() const override;
	private:
		Result m_result;
	};

	inline GpioRegularResponse::GpioRegularResponse(const Result& result): m_result(result) {

	}

	inline typename GpioRegularResponse::Type GpioRegularResponse::type() const {
		return Type::REGULAR;
	}

	inline typename GpioRegularResponse::Result GpioRegularResponse::result() const {
		return m_result;
	}
}

#endif // GPIO_REGULAR_RESPONSE_HPP