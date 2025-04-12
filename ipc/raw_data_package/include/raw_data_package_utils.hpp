#ifndef	RAW_DATA_PACKAGE_UTILS_HPP
#define	RAW_DATA_PACKAGE_UTILS_HPP

#include <cstddef>
#include <stdexcept>

#include "raw_data_package_descriptor.hpp"

namespace ipc {
	inline typename RawDataPackageDescriptor::RawData serialize_package_size(const RawDataPackageDescriptor& package_descriptor, const std::size_t& package_data_size) {
		enum: std::size_t { BITS_IN_BYTE = 8UL };
		auto encoded_size = RawDataPackageDescriptor::RawData(package_descriptor.encoded_size_length(), 0UL);
		for (auto i = 0; i < package_descriptor.encoded_size_length(); ++i) {
			const auto less_sign_byte = static_cast<char>((package_data_size >> BITS_IN_BYTE * i) & 0xFF);
			encoded_size[i] = less_sign_byte;
		}
		return encoded_size;
	}

	inline std::size_t parse_package_size(const RawDataPackageDescriptor& package_descriptor, const RawDataPackageDescriptor::RawData& package_data_size_raw_data) {
		enum: std::size_t { BITS_IN_BYTE = 8UL };
		if (package_descriptor.encoded_size_length() != package_data_size_raw_data.size()) {
			throw std::invalid_argument("invalid raw data size");
		}
		auto decoded_size = std::size_t(0UL);
		for (auto i = 0; i < package_descriptor.encoded_size_length(); ++i) {
			decoded_size <<= BITS_IN_BYTE;
			decoded_size |= static_cast<std::size_t>(package_data_size_raw_data[package_descriptor.encoded_size_length() - 1 - i]);
		}
		return decoded_size;
	}
}

#endif // RAW_DATA_PACKAGE_UTILS_HPP