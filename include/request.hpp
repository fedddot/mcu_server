#ifndef	REQUEST_HPP
#define	REQUEST_HPP

#include <string>
#include <vector>

#include "object.hpp"

namespace server {
	class Request {
	public:
		enum class Method: int {
			CREATE,
			READ,
			UPDATE,
			DELETE
		};
		using Path = std::vector<std::string>;
		using Body = Object;
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

	inline typename Request::Path Request::path() const {
		return m_path;
	}

	inline typename Request::Body Request::body() const {
		return m_body;
	}
}

#endif // REQUEST_HPP