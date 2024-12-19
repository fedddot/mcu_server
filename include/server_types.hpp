#ifndef	SERVER_TYPES_HPP
#define	SERVER_TYPES_HPP

#include <string>
#include <vector>

#include "object.hpp"

namespace server {
	using ResourceId = std::string;
	using Path = std::vector<ResourceId>;
	using Body = Object;
	
	enum class ResponseCode: int {
		OK,
		METHOD_NOT_ALLOWED,
		NOT_FOUND,
		BAD_REQUEST,
		TIMEOUT,
		UNSPECIFIED
	};
}

#endif // SERVER_TYPES_HPP