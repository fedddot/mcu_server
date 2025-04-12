#ifndef	SHARED_IPC_DATA_READER_HPP
#define	SHARED_IPC_DATA_READER_HPP

#include <memory>
#include <optional>
#include <stdexcept>

#include "ipc_data_reader.hpp"

namespace ipc {
	template <typename IpcData>
	class SharedIpcDataReader: public IpcDataReader<IpcData> {
	public:
		SharedIpcDataReader(IpcDataReader<IpcData> *instance_ptr);
		SharedIpcDataReader(const SharedIpcDataReader&) = default;
		SharedIpcDataReader& operator=(const SharedIpcDataReader&) = default;

		std::optional<IpcData> read() override;
	private:
		std::shared_ptr<IpcDataReader<IpcData>> m_instance_ptr;
	};

	template <typename IpcData>
	inline SharedIpcDataReader<IpcData>::SharedIpcDataReader(IpcDataReader<IpcData> *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename IpcData>
	inline std::optional<IpcData> SharedIpcDataReader<IpcData>::read() {
		return m_instance_ptr->read();
	}
}

#endif // SHARED_IPC_DATA_READER_HPP