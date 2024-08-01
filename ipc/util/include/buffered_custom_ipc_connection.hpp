#ifndef	BUFFERED_CUSTOM_IPC_CONNECTION_HPP
#define	BUFFERED_CUSTOM_IPC_CONNECTION_HPP

#include "buffered_ipc_connection.hpp"
namespace mcu_ipc_utl {
	template <typename Tdata>
	class BufferedCustomIpcConnection: public mcu_ipc::IpcConnection<Tdata> {
	public:
		virtual ~IpcConnection() noexcept = default;
		virtual bool readable() const = 0;
		virtual Tdata read() = 0;
		virtual void send(const Tdata& data) const = 0;
	};
}

#endif // BUFFERED_CUSTOM_IPC_CONNECTION_HPP