#include <climits>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "package_utils.hpp"

using namespace ipc;

std::vector<std::uint8_t> ipc::serialize_package_size(const std::size_t& package_size, const std::size_t& encoded_size_length) {
	auto encoded_size = std::vector<std::uint8_t>(encoded_size_length, 0UL);
	for (auto i = 0; i < encoded_size_length; ++i) {
		const auto less_sign_byte = static_cast<char>((package_size >> CHAR_BIT * i) & 0xFF);
		encoded_size[i] = less_sign_byte;
	}
	return encoded_size;
}

std::size_t ipc::parse_package_size(const std::vector<std::uint8_t>& package_size_data) {
	const auto package_size_data_size = package_size_data.size();
	if (sizeof(std::size_t) < package_size_data_size) {
		throw std::invalid_argument("invalid raw data size");
	}
	auto decoded_size = std::size_t(0UL);
	for (auto i = 0; i < package_size_data_size; ++i) {
		decoded_size <<= CHAR_BIT;
		decoded_size |= static_cast<std::size_t>(package_size_data[package_size_data_size - 1 - i]);
	}
	return decoded_size;
}