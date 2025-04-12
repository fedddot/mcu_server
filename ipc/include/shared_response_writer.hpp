#ifndef	SHARED_IPC_DATA_WRITER_HPP
#define	SHARED_IPC_DATA_WRITER_HPP

#include <memory>
#include <stdexcept>

#include "ipc_data_writer.hpp"

namespace ipc {
	template <typename IpcData>
	class SharedIpcDataWriter: public IpcDataWriter<IpcData> {
	public:
		SharedIpcDataWriter(IpcDataWriter<IpcData> *instance_ptr);
		SharedIpcDataWriter(const SharedIpcDataWriter&) = default;
		SharedIpcDataWriter& operator=(const SharedIpcDataWriter&) = default;

		void write(const IpcData& data) const override;
	private:
		std::shared_ptr<IpcDataWriter<IpcData>> m_instance_ptr;
	};

	template <typename IpcData>
	inline SharedIpcDataWriter<IpcData>::SharedIpcDataWriter(IpcDataWriter<IpcData> *instance_ptr): m_instance_ptr(instance_ptr) {
		if (!m_instance_ptr) {
			throw std::invalid_argument("invalid instance ptr received");
		}
	}

	template <typename IpcData>
	inline void SharedIpcDataWriter<IpcData>::write(const IpcData& data) const {
		m_instance_ptr->write(data);
	}
}

#endif // SHARED_IPC_DATA_WRITER_HPP