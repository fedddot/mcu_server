#ifndef	GPIO_WRITE_REQUEST_HPP
#define	GPIO_WRITE_REQUEST_HPP

#include "gpio.hpp"
#include "gpio_request.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioWriteRequest: public GpioRequest<Tgpio_id> {
	public:
		GpioWriteRequest(const Tgpio_id& id, const Gpio::State& state);
		GpioWriteRequest(const GpioWriteRequest&) = default;
		GpioWriteRequest& operator=(const GpioWriteRequest&) = default;

		Gpio::State state() const;
	private:
		Gpio::State m_state;
	};

	template <typename Tgpio_id>
	inline GpioWriteRequest<Tgpio_id>::GpioWriteRequest(const Tgpio_id& id, const Gpio::State& state): GpioRequest<Tgpio_id>(GpioRequest<Tgpio_id>::Operation::WRITE, id), m_state(state) {

	}

	template <typename Tgpio_id>
	inline typename Gpio::State GpioWriteRequest<Tgpio_id>::state() const {
		return m_state;
	}
}

#endif // GPIO_WRITE_REQUEST_HPP