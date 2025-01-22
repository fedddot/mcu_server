#ifndef	IPC_REQUEST_HANDLER_HPP
#define	IPC_REQUEST_HANDLER_HPP

namespace ipc {
	template <typename Request, typename Response>
	class IpcRequestHandler {
	public:
		virtual ~IpcRequestHandler() noexcept = default;
		virtual Response handle(const Request& request) const = 0;
	};
}

#endif // IPC_REQUEST_HANDLER_HPP