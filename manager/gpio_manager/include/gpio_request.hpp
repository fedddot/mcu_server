#ifndef	GPIO_REQUEST_HPP
#define	GPIO_REQUEST_HPP

namespace manager {
	template <typename Tgpio_id>
	class GpioRequest {
	public:
		enum class Operation: int {
			CREATE,
			READ,
			WRITE,
			DELETE
		};
		GpioRequest(const Operation& operation, const Tgpio_id& id);
		GpioRequest(const GpioRequest&) = default;
		GpioRequest& operator=(const GpioRequest&) = default;
		virtual ~GpioRequest() noexcept = default;
		Operation operation() const;
		Tgpio_id id() const;
	private:
		Operation m_operation;
		Tgpio_id m_id;
	};

	template <typename Tgpio_id>
	inline GpioRequest<Tgpio_id>::GpioRequest(const Operation& operation, const Tgpio_id& id): m_operation(operation), m_id(id) {

	}

	template <typename Tgpio_id>
	inline typename GpioRequest<Tgpio_id>::Operation GpioRequest<Tgpio_id>::operation() const {
		return m_operation;
	}

	template <typename Tgpio_id>
	inline Tgpio_id GpioRequest<Tgpio_id>::id() const {
		return m_id;
	}
}

#endif // GPIO_REQUEST_HPP