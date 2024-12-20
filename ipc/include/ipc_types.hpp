#ifndef	IPC_TYPES_HPP
#define	IPC_TYPES_HPP

#include <cstddef>
#include <vector>

namespace ipc {
	using Package = std::vector<char>;
	using PackageSize = std::size_t;
}

#endif // IPC_TYPES_HPP