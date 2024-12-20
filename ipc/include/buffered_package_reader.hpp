#ifndef	BUFFERED_PACKAGE_READER_HPP
#define	BUFFERED_PACKAGE_READER_HPP

namespace ipc {
	template <typename Tdata>
	class BufferedPackageReader {
	public:
		virtual ~BufferedPackageReader() noexcept = default;
		virtual bool readable() const = 0;
		virtual Tdata read() const = 0;
	};
}

#endif // BUFFERED_PACKAGE_READER_HPP