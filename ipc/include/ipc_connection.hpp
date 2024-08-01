#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

namespace mcu_ipc {
	template <typename Tdata>
	class IpcConnection {
	public:
		virtual ~IpcConnection() noexcept = default;
		virtual bool readable() const = 0;
		virtual Tdata read() = 0;
		virtual void send(const Tdata& data) const = 0;
	};
}

#endif // IPC_CONNECTION_HPP