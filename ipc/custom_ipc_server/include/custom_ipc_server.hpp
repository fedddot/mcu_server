#ifndef	CUSTOM_IPC_SERVER_HPP
#define	CUSTOM_IPC_SERVER_HPP

#include <stdexcept>

#include "ipc_server.hpp"

namespace ipc {
	/// @brief Customizable synchronous IPC server aggregating an external FIFO buffer
	template <typename Request, typename Response>
	class CustomIpcServer: IpcServer<Request, Response> {
	public:
		using RequestCallback = typename IpcServer<Request, Response>::RequestCallback;
		CustomIpcServer();
		CustomIpcServer(const CustomIpcServer&) = delete;
		CustomIpcServer& operator=(const CustomIpcServer&) = delete;
		
		void serve_once(const RequestCallback& request_callback) override;
	};

	template <typename Request, typename Response>
	inline CustomIpcServer<Request, Response>::CustomIpcServer() {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	template <typename Request, typename Response>
	inline void CustomIpcServer<Request, Response>::serve_once(const RequestCallback& request_callback) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // CUSTOM_IPC_SERVER_HPP