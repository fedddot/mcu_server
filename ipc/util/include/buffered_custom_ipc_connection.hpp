#ifndef	BUFFERED_CUSTOM_IPC_CONNECTION_HPP
#define	BUFFERED_CUSTOM_IPC_CONNECTION_HPP

#include "ipc_connection.hpp"
#include <cstddef>

namespace mcu_ipc_utl {
	template <typename Tdata>
	class BufferedCustomIpcConnection: public mcu_ipc::IpcConnection<Tdata> {
	public:
		BufferedCustomIpcConnection(const Tdata& head, const Tdata& tail, const std::size_t& max_buff_size);
		BufferedCustomIpcConnection(const BufferedCustomIpcConnection& other);
		BufferedCustomIpcConnection& operator=(const BufferedCustomIpcConnection& other) = delete;
		
		virtual bool readable() const;
		virtual Tdata read() = 0;
		virtual void send(const Tdata& data) const = 0;
	};
}

#endif // BUFFERED_CUSTOM_IPC_CONNECTION_HPP