#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

#include "request.hpp"
#include "response.hpp"

namespace server {
	class IpcConnection {
	public:
		virtual ~IpcConnection() noexcept = default;
		virtual bool readable() const = 0;
		virtual Request read() = 0;
		virtual void send(const Response& response) const = 0;
	};
}

#endif // IPC_CONNECTION_HPP