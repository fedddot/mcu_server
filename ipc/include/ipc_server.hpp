#ifndef	IPC_SERVER_HPP
#define	IPC_SERVER_HPP

#include <functional>

namespace ipc {
	/// @brief Synchronous IPC server
	template <typename Request, typename Response>
	class IpcServer {
	public:
		using RequestCallback = std::function<Response(const Request&)>;
		virtual ~IpcServer() noexcept = default;

		/// @brief Runs only one blocking service iteration - if there is an IPC message to serve, received callback will be invoke, otherwise anything happens
		/// @param request_callback - a callback being invoked if an IPC message received
		virtual void serve_once(const RequestCallback& request_callback) = 0;
	};
}

#endif // IPC_SERVER_HPP