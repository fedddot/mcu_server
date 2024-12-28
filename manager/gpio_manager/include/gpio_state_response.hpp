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
		Gpio::State state() const;
	private:
		Gpio::State m_state;
	};

	inline GpioStateResponse::GpioStateResponse(const Result& result, const Gpio::State& state): GpioResponse(Type::STATE_RESPONSE, result), m_state(state) {

	}

	inline typename Gpio::State GpioStateResponse::state() const {
		return m_state;
	}
}

#endif // GPIO_STATE_RESPONSE_HPP