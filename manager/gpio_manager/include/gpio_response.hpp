#ifndef	GPIO_RESPONSE_HPP
#define	GPIO_RESPONSE_HPP

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
		GpioResponse(const Type& type, const Result& result);
		GpioResponse(const GpioResponse&) = default;
		GpioResponse& operator=(const GpioResponse&) = default;
		virtual ~GpioResponse() noexcept = default;
		Type type() const;
		Result result() const;
	private:
		Type m_type;
		Result m_result;
	};

	inline GpioResponse::GpioResponse(const Type& type, const Result& result): m_type(type), m_result(result) {

	}

	inline typename GpioResponse::Type GpioResponse::type() const {
		return m_type;
	}

	inline typename GpioResponse::Result GpioResponse::result() const {
		return m_result;
	}
}

#endif // GPIO_RESPONSE_HPP