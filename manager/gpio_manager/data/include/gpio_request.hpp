#ifndef	GPIO_REQUEST_HPP
#define	GPIO_REQUEST_HPP

#include <memory>
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
		virtual ~GpioRequest() noexcept = default;
		virtual Operation operation() const = 0;
		virtual Tgpio_id id() const = 0;
	};

	template <typename Tgpio_id>
	using GpioRequestWrapper = std::shared_ptr<GpioRequest<Tgpio_id>>;
}

#endif // GPIO_REQUEST_HPP