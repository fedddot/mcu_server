#ifndef	GPIO_STATE_RESPONSE_HPP
#define	GPIO_STATE_RESPONSE_HPP

#include "gpio.hpp"
#include "gpio_response.hpp"

namespace manager {
	class GpioStateResponse: public GpioResponse {
	public:
		GpioStateResponse(const Result& result, const Gpio::State& state);
		GpioStateResponse(const GpioStateResponse&) = default;
		GpioStateResponse& operator=(const GpioStateResponse&) = default;
		Type type() const override;
		Result result() const override;
		Gpio::State state() const;
	private:
		Result m_result;
		Gpio::State m_state;
	};

	inline GpioStateResponse::GpioStateResponse(const Result& result, const Gpio::State& state): m_result(result), m_state(state) {

	}

	inline typename GpioStateResponse::Type GpioStateResponse::type() const {
		return Type::STATE_RESPONSE;
	}

	inline typename GpioStateResponse::Result GpioStateResponse::result() const {
		return m_result;
	}

	inline typename Gpio::State GpioStateResponse::state() const {
		return m_state;
	}
}

#endif // GPIO_STATE_RESPONSE_HPP