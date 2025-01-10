#ifndef	IPC_SERVER_FACTORY_HPP
#define	IPC_SERVER_FACTORY_HPP

#include <stdexcept>

#include "ipc_config.hpp"
#include "ipc_server.hpp"
#include "test_ipc_server.hpp"
#include "http_ipc_server.hpp"

namespace ipc {
	template <typename Tincoming, typename Toutgoing>
	class IpcServerFactory {
	public:
		IpcServerFactory() = default;
		virtual ~IpcServerFactory() noexcept = default;
		IpcServer<Tincoming, Toutgoing> *operator()(const IpcConfig& config) const;
	private:
		template <typename Tconfig>
		static const Tconfig& cast_config(const IpcConfig& config);
	};

	template <typename Tincoming, typename Toutgoing>
	inline IpcServer<Tincoming, Toutgoing> *IpcServerFactory<Tincoming, Toutgoing>::operator()(const IpcConfig& config) const {
		if ("ipc.server.test" == config.type()) {
			using namespace ipc_tests;
			return new TestIpcServer<Tincoming, Toutgoing>(cast_config<TestIpcConfig<Toutgoing>>(config));
		}
		if ("ipc.server.http" == config.type()) {
			return new HttpIpcServer<Tincoming, Toutgoing>(cast_config<HttoIpcServerConfig<Tincoming, Toutgoing>>(config));
		}
		throw std::invalid_argument("insupported ipc server type");
	}

	template <typename Tincoming, typename Toutgoing>
	template <typename Tconfig>
	const Tconfig& IpcServerFactory<Tincoming, Toutgoing>::cast_config(const IpcConfig& config) {
		try {
			return dynamic_cast<const Tconfig&>(config);
		} catch (...) {
			throw std::runtime_error("failed to cast ipc server config into target type: " + config.type());
		}
	}
}

#endif // IPC_SERVER_FACTORY_HPP