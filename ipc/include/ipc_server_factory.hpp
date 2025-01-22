#ifndef	IPC_SERVER_FACTORY_HPP
#define	IPC_SERVER_FACTORY_HPP

#include <stdexcept>

#include "buffered_ipc_server.hpp"
#include "http_ipc_server.hpp"
#include "ipc_config.hpp"
#include "ipc_server.hpp"

namespace ipc {
	template <typename Request, typename Response, typename RawData>
	class IpcServerFactory {
	public:
		IpcServerFactory() = default;
		virtual ~IpcServerFactory() noexcept = default;
		IpcServer<Request, Response> *operator()(const IpcConfig& config) const;
	private:
		template <typename Config>
		static const Config& cast_config(const IpcConfig& config);
	};

	template <typename Request, typename Response, typename RawData>
	inline IpcServer<Request, Response> *IpcServerFactory<Request, Response, RawData>::operator()(const IpcConfig& config) const {
		if ("ipc.server.buffered" == config.type()) {
			return new BufferedIpcServer<Request, Response, RawData>(cast_config<BufferedIpcServerConfig<Request, Response, RawData>>(config));
		}
		if ("ipc.server.http" == config.type()) {
			return new HttpIpcServer<Request, Response>(cast_config<HttpIpcServerConfig<Request, Response>>(config));
		}
		throw std::invalid_argument("insupported ipc server type");
	}

	template <typename Request, typename Response, typename RawData>
	template <typename Config>
	const Config& IpcServerFactory<Request, Response, RawData>::cast_config(const IpcConfig& config) {
		try {
			return dynamic_cast<const Config&>(config);
		} catch (...) {
			throw std::runtime_error("failed to cast ipc server config into target type: " + config.type());
		}
	}
}

#endif // IPC_SERVER_FACTORY_HPP