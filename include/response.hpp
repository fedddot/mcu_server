#ifndef	RESPONSE_HPP
#define	RESPONSE_HPP

#include "server_types.hpp"

namespace server {
	class Response {
	public:
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

	inline ResponseCode Response::code() const {
		return m_code;
	}

	inline Body Response::body() const {
		return m_body;
	}
}

#endif // RESPONSE_HPP