#ifndef	GPIO_RESPONSE_HPP
#define	GPIO_RESPONSE_HPP

namespace manager {
	class GpioResponse {
	public:
		enum class GpioResult: int {
			SUCCESS,
			FAILURE
		};
		GpioResponse(const GpioResult& result);
		GpioResponse(const GpioResponse&) = default;
		GpioResponse& operator=(const GpioResponse&) = default;
		virtual ~GpioResponse() noexcept = default;
		GpioResult result() const;
	private:
		GpioResult m_result;
	};

	inline GpioResponse::GpioResponse(const GpioResult& result): m_result(result) {

	}

	inline typename GpioResponse::GpioResult GpioResponse::result() const {
		return m_result;
	}
}

#endif // GPIO_RESPONSE_HPP