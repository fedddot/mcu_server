#ifndef	GPIO_DELETE_REQUEST_HPP
#define	GPIO_DELETE_REQUEST_HPP

#include "gpio_request.hpp"

namespace manager {
	template <typename Tgpio_id>
	class GpioDeleteRequest: public GpioRequest<Tgpio_id> {
	public:
		using Operation = typename GpioRequest<Tgpio_id>::Operation;
		GpioDeleteRequest(const Tgpio_id& id);
		GpioDeleteRequest(const GpioDeleteRequest&) = default;
		GpioDeleteRequest& operator=(const GpioDeleteRequest&) = default;
		
		Operation operation() const override;
		Tgpio_id id() const override;
	private:
		Tgpio_id m_id;
	};

	template <typename Tgpio_id>
	inline GpioDeleteRequest<Tgpio_id>::GpioDeleteRequest(const Tgpio_id& id): m_id(id) {

	}

	template <typename Tgpio_id>
	inline typename GpioDeleteRequest<Tgpio_id>::Operation GpioDeleteRequest<Tgpio_id>::operation() const {
		return Operation::DELETE;
	}

	template <typename Tgpio_id>
	inline Tgpio_id GpioDeleteRequest<Tgpio_id>::id() const {
		return m_id;
	}
}

#endif // GPIO_DELETE_REQUEST_HPP