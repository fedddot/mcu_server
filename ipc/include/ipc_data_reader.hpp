#ifndef	IPC_DATA_READER_HPP
#define	IPC_DATA_READER_HPP

#include <optional>

#include "ipc_instance.hpp"

namespace ipc {
	template <typename IpcData>
	class IpcDataReader {
	public:
		virtual ~IpcDataReader() noexcept = default;
		virtual std::optional<Instance<IpcData>> read() = 0;
	};
}

#endif // IPC_DATA_READER_HPP