#ifndef	CLONABLE_IPC_DATA_WRITER_HPP
#define	CLONABLE_IPC_DATA_WRITER_HPP

#include "ipc_data_writer.hpp"

namespace ipc {
	template <typename IpcData>
	class ClonableIpcDataWriter: public IpcDataWriter<IpcData> {
	public:
		virtual IpcDataWriter<IpcData> *clone() const = 0;
	};
}

#endif // CLONABLE_IPC_DATA_WRITER_HPP