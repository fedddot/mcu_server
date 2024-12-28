#ifndef	GPIO_RESPONSE_HPP
#define	GPIO_RESPONSE_HPP

#include <memory>

namespace manager {
	class GpioResponse {
	public:
		enum class Type: int {
			REGULAR,
			STATE_RESPONSE
		};
		enum class Result: int {
			SUCCESS,
			FAILURE
		};
		virtual ~GpioResponse() noexcept = default;
		virtual Type type() const = 0;
		virtual Result result() const = 0;
	};
	using GpioResponseWrapper = std::shared_ptr<GpioResponse>;
}

#endif // GPIO_RESPONSE_HPP