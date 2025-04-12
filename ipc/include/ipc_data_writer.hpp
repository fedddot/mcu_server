#ifndef	IPC_DATA_WRITER_HPP
#define	IPC_DATA_WRITER_HPP

namespace ipc {
	template <typename IpcData>
	class IpcDataWriter {
	public:
		virtual ~IpcDataWriter() noexcept = default;
		virtual void write(const IpcData& data) const = 0;
	};
}

#endif // IPC_DATA_WRITER_HPP