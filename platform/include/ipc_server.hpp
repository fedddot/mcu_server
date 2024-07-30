#ifndef	IPC_SERVER_HPP
#define	IPC_SERVER_HPP

namespace mcu_platform {	

	template <typename Tmsg>
	class IpcServer {
	public:
		virtual ~IpcServer() noexcept = default;
		virtual bool poll(int timeout_ms) const = 0;
		virtual Tmsg receive() = 0;
		virtual void send(const Tmsg& data) const = 0;
	};
}

#endif // IPC_SERVER_HPP