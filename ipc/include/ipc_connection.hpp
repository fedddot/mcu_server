#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

#include "request.hpp"
#include "response.hpp"

namespace ipc {
	class IpcConnection {
	public:
		virtual ~IpcConnection() noexcept = default;
		virtual bool readable() const = 0;
		virtual server::Request read() = 0;
		virtual void send(const server::Response& response) const = 0;
	};
}

#endif // IPC_CONNECTION_HPP