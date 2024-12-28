#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

#include "data_reader.hpp"
#include "data_writer.hpp"

namespace ipc {
	template <typename Tincoming, typename Toutgoing>
	class IpcConnection {
	public:
		virtual ~IpcConnection() noexcept = default;
		virtual const DataReader<Tincoming> *reader() const = 0;
		virtual const DataWriter<Toutgoing> *writer() const = 0;
	};
}

#endif // IPC_CONNECTION_HPP