#ifndef	GPIO_READ_REQUEST_HPP
#define	GPIO_READ_REQUEST_HPP

#include "gpio_request.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioReadRequest: public GpioRequest<Tgpio_id> {
	public:
		using Operation = typename GpioRequest<Tgpio_id>::Operation;
		GpioReadRequest(const Tgpio_id& id);
		GpioReadRequest(const GpioReadRequest&) = default;
		GpioReadRequest& operator=(const GpioReadRequest&) = default;
		
		Operation operation() const override;
		Tgpio_id id() const override;
	private:
		Tgpio_id m_id;
	};

	template <typename Tgpio_id>
	inline GpioReadRequest<Tgpio_id>::GpioReadRequest(const Tgpio_id& id): m_id(id) {

	}

	template <typename Tgpio_id>
	inline typename GpioReadRequest<Tgpio_id>::Operation GpioReadRequest<Tgpio_id>::operation() const {
		return Operation::READ;
	}

	template <typename Tgpio_id>
	inline Tgpio_id GpioReadRequest<Tgpio_id>::id() const {
		return m_id;
	}
}

#endif // GPIO_READ_REQUEST_HPP