#ifndef	DATA_WRITER_HPP
#define	DATA_WRITER_HPP

namespace ipc {
	template <typename Tdata>
	class DataWriter {
	public:
		virtual ~DataWriter() noexcept = default;
		virtual void write(const Tdata& data) const = 0;
	};
}

#endif // DATA_WRITER_HPP