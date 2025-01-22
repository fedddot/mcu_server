#ifndef	IPC_SERVER_HPP
#define	IPC_SERVER_HPP

#include "ipc_request_handler.hpp"

namespace ipc {
	template <typename Request, typename Response>
	class IpcServer {
	public:
		virtual ~IpcServer() noexcept = default;
		virtual void serve(IpcRequestHandler<Request, Response> *request_handler) = 0;
		virtual void stop() = 0;
	};
}

#endif // IPC_SERVER_HPP