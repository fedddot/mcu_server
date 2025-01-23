#ifndef	IPC_SERVER_HPP
#define	IPC_SERVER_HPP

#include <functional>

namespace ipc {
	template <typename Request, typename Response>
	class IpcServer {
	public:
		using Handler = std::function<Response(const Request&)>;
		virtual ~IpcServer() noexcept = default;
		virtual void serve(const Handler& handler) = 0;
		virtual void stop() = 0;
	};
}

#endif // IPC_SERVER_HPP