#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

#include <functional>
#include <string>

#include "request.hpp"
#include "response.hpp"

namespace ipc {
	class IpcConnection {
	public:
		using Callback = std::function<void(const server::Request& request)>;
		virtual ~IpcConnection() noexcept = default;
		virtual void subscribe(const std::string& id, const Callback& cb) = 0;
		virtual void unsubscribe(const std::string& id) = 0;
		virtual bool is_subscribed(const std::string& id) const = 0;
		virtual void send(const server::Response& response) const = 0;
	};
}

#endif // IPC_CONNECTION_HPP