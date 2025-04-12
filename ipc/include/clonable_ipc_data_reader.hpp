#ifndef	CLONABLE_IPC_DATA_READER_HPP
#define	CLONABLE_IPC_DATA_READER_HPP

#include "ipc_data_reader.hpp"

namespace ipc {
	template <typename IpcData>
	class ClonableIpcDataReader: public IpcDataReader<IpcData> {
	public:
		virtual IpcDataReader<IpcData> *clone() const = 0;
	};
}

#endif // CLONABLE_IPC_DATA_READER_HPP