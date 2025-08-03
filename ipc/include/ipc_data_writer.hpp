#ifndef	IPC_DATA_WRITER_HPP
#define	IPC_DATA_WRITER_HPP

namespace ipc {
	template <typename Data>
	class DataWriter {
	public:
		virtual ~DataWriter() noexcept = default;
		virtual void write(const Data& data) const = 0;
	};
}

#endif // IPC_DATA_WRITER_HPP