#ifndef	RAW_DATA_PACKAGE_UTILS_HPP
#define	RAW_DATA_PACKAGE_UTILS_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

namespace ipc {
	std::vector<std::uint8_t> serialize_package_size(const std::size_t& package_size, const std::size_t& encoded_size_length);
	std::size_t parse_package_size(const std::vector<std::uint8_t>& package_size_data);
}

#endif // RAW_DATA_PACKAGE_UTILS_HPP