#ifndef	RAW_DATA_PACKAGE_UTILS_HPP
#define	RAW_DATA_PACKAGE_UTILS_HPP

#include <cstddef>

#include "ipc_data.hpp"
#include "raw_data_package_descriptor.hpp"

namespace ipc {
	RawData serialize_package_size(const RawDataPackageDescriptor& package_descriptor, const std::size_t& package_data_size);
	std::size_t parse_package_size(const RawDataPackageDescriptor& package_descriptor, const RawData& package_data_size_raw_data);
}

#endif // RAW_DATA_PACKAGE_UTILS_HPP