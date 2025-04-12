#ifndef	RAW_DATA_PACKAGE_UTILS_HPP
#define	RAW_DATA_PACKAGE_UTILS_HPP

#include <cstddef>
#include <stdexcept>

#include "raw_data_package_descriptor.hpp"

namespace ipc {
	inline typename RawDataPackageDescriptor::RawData serialize_package_size(const RawDataPackageDescriptor& package_descriptor, const std::size_t& package_data_size) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

	inline std::size_t parse_package_size(const RawDataPackageDescriptor& package_descriptor, const RawDataPackageDescriptor::RawData& package_data_size_raw_data) {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // RAW_DATA_PACKAGE_UTILS_HPP