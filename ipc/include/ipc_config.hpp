#ifndef	IPC_CONFIG_HPP
#define	IPC_CONFIG_HPP

#include <string>

namespace ipc {
	class IpcConfig {
	public:
		virtual ~IpcConfig() noexcept = default;
		virtual std::string type() const = 0;
	};
}

#endif // IPC_CONFIG_HPP