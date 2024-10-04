#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

#include <functional>

#include "request.hpp"
#include "response.hpp"

namespace ipc {
	class IpcConnection {
	public:
		using Callback = std::function<void(const server::Request& request)>;
		virtual ~IpcConnection() noexcept = default;
		virtual void set_callback(const Callback& cb) = 0;
		virtual void send(const server::Response& response) const = 0;
	};
}

#endif // IPC_CONNECTION_HPP