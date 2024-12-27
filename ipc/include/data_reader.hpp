#ifndef	DATA_READER_HPP
#define	DATA_READER_HPP

namespace ipc {
	template <typename Tdata>
	class DataReader {
	public:
		virtual ~DataReader() noexcept = default;
		virtual bool readable() const = 0;
		virtual Tdata read() const = 0;
	};
}

#endif // DATA_READER_HPP