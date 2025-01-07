#ifndef	IPC_SERVER_HPP
#define	IPC_SERVER_HPP

namespace ipc {
	template <typename Tincoming, typename Toutgoing>
	class IpcServer {
	public:
		virtual ~IpcServer() noexcept = default;
		virtual bool readable() const = 0;
		virtual Tincoming read() = 0;
		virtual void write(const Toutgoing& data) const = 0;
	};
}

#endif // IPC_SERVER_HPP