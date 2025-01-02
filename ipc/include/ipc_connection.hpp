#ifndef	IPC_CONNECTION_HPP
#define	IPC_CONNECTION_HPP

namespace ipc {
	template <typename Tincoming, typename Toutgoing>
	class IpcConnection {
	public:
		virtual ~IpcConnection() noexcept = default;
		virtual bool readable() const = 0;
		virtual Tincoming read() = 0;
		virtual void write(const Toutgoing& data) const = 0;
	};
}

#endif // IPC_CONNECTION_HPP