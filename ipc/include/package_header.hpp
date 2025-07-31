#ifndef	PACKAGE_HEADER_HPP
#define	PACKAGE_HEADER_HPP

#include <cstddef>

namespace ipc {
	template <typename T>
	class PackageHeader {
	public:
		virtual ~PackageHeader() noexcept = default;
		virtual std::size_t package_size() const = 0;
	};
}

#endif // PACKAGE_HEADER_HPP