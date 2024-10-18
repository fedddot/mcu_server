#ifndef	SERVER_EXCEPTION_HPP
#define	SERVER_EXCEPTION_HPP

#include <exception>
#include <string>

#include "server_types.hpp"
#include "string.hpp"

namespace server {
	class ServerException: public std::exception {
	public:
		ServerException(const ResponseCode& code, const std::string& what);
		ServerException(const ServerException&) = default;
		ServerException& operator=(const ServerException&) = default;
		
		virtual Body body() const;
		virtual ResponseCode code() const;
		const char *what() const noexcept override;
	private:
		ResponseCode m_code;
		std::string m_what;
	};

	inline ServerException::ServerException(const ResponseCode& code, const std::string& what): m_code(code), m_what(what) {

	}

	inline Body ServerException::body() const {
		Body body;
		body.add("what", String(m_what));
		return body;
	}

	inline ResponseCode ServerException::code() const {
		return m_code;
	}

	inline const char *ServerException::what() const noexcept {
		return m_what.c_str();
	}
}

#endif // SERVER_EXCEPTION_HPP