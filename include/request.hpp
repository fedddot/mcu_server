#ifndef	REQUEST_HPP
#define	REQUEST_HPP

#include "server_types.hpp"

namespace server {
	class Request {
	public:
		enum class Method: int {
			CREATE,
			READ,
			UPDATE,
			DELETE
		};
		Request(const Method& method, const Path& path, const Body& body);
		Request(const Request&) = default;
		Request& operator=(const Request&) = default;

		virtual ~Request() noexcept = default;
		Method method() const;
		Path path() const;
		Body body() const;
	private:
		Method m_method;
		Path m_path;
		Body m_body;
	};

	inline Request::Request(const Method& method, const Path& path, const Body& body): m_method(method), m_path(path), m_body(body) {

	}

	inline typename Request::Method Request::method() const {
		return m_method;
	}

	inline Path Request::path() const {
		return m_path;
	}

	inline Body Request::body() const {
		return m_body;
	}
}

#endif // REQUEST_HPP