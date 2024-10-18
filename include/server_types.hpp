#ifndef	SERVER_TYPES_HPP
#define	SERVER_TYPES_HPP

#include <string>
#include <vector>

#include "object.hpp"

namespace server {
	using Path = std::vector<std::string>;
	using Body = Object;
	
	enum class ResponseCode: int {
		OK,
		METHOD_NOT_ALLOWED,
		NOT_FOUND,
		BAD_REQUEST,
		UNSPECIFIED
	};
}

#endif // SERVER_TYPES_HPP