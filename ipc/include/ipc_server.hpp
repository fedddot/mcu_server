#ifndef	IPC_SERVER_HPP
#define	IPC_SERVER_HPP

#include "ipc_option.hpp"

namespace ipc {
	template <typename Request, typename Response>
	class IpcServer {
	public:
		virtual ~IpcServer() noexcept = default;
		virtual Option<Request> read() = 0;
		virtual void write(const Response& data) const = 0;
	};
}

#endif // IPC_SERVER_HPP