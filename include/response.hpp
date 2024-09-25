#ifndef	RESPONSE_HPP
#define	RESPONSE_HPP

#include "object.hpp"

namespace server {
	class Response {
	public:
		enum class ResponseCode: int {
			OK
		};
		using Body = Object;
		Response(const ResponseCode& code, const Body& body);
		Response(const Response&) = default;
		Response& operator=(const Response&) = default;

		virtual ~Response() noexcept = default;
		ResponseCode code() const;
		Body body() const;
	private:
		ResponseCode m_code;
		Body m_body;
	};

	inline Response::Response(const ResponseCode& code, const Body& body): m_code(code), m_body(body) {

	}

	inline typename Response::ResponseCode Response::code() const {
		return m_code;
	}

	inline typename Response::Body Response::body() const {
		return m_body;
	}
}

#endif // RESPONSE_HPP