#ifndef	GPIO_RESPONSE_HPP
#define	GPIO_RESPONSE_HPP

namespace manager {
	class GpioResponse {
	public:
		enum class Result: int {
			SUCCESS,
			FAILURE
		};
		GpioResponse(const Result& result);
		GpioResponse(const GpioResponse&) = default;
		GpioResponse& operator=(const GpioResponse&) = default;
		virtual ~GpioResponse() noexcept = default;
		Result result() const;
	private:
		Result m_result;
	};

	inline GpioResponse::GpioResponse(const Result& result): m_result(result) {

	}

	inline typename GpioResponse::Result GpioResponse::result() const {
		return m_result;
	}
}

#endif // GPIO_RESPONSE_HPP