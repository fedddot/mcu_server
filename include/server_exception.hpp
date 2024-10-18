#ifndef	SERVER_EXCEPTION_HPP
#define	SERVER_EXCEPTION_HPP

#include <exception>

#include "server_types.hpp"

namespace server {
	class ServerException: public std::exception {
	public:
		virtual Body body() const = 0;
		virtual ResponseCode code() const = 0;
	};
}

#endif // SERVER_EXCEPTION_HPP