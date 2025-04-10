#ifndef	SIZED_PACKAGE_INFRA_HPP
#define	SIZED_PACKAGE_INFRA_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace ipc {
	class SizedPackageInfra {
	public:
		static std::vector<char> encode_size(const std::size_t& size);
		static std::size_t decode_size(const std::vector<char>& raw_size);
	private:
		enum: std::size_t {
			ENCODED_SIZE_BYTES = sizeof(std::size_t),
			BITS_IN_BYTE = 8UL,
		};
	};

	inline std::vector<char> SizedPackageInfra::encode_size(const std::size_t& size) {
		auto encoded_size = std::vector<char>(ENCODED_SIZE_BYTES, 0UL);
		for (auto i = 0; i < ENCODED_SIZE_BYTES; ++i) {
			const auto less_sign_byte = static_cast<char>(
				(size >> BITS_IN_BYTE * i) & 0xFF
			);
			encoded_size[i] = less_sign_byte;
		}
		return encoded_size;
	}

	inline std::size_t SizedPackageInfra::decode_size(const std::vector<char>& raw_size) {
		if (ENCODED_SIZE_BYTES != raw_size.size()) {
			throw std::invalid_argument("invalid raw data size");
		}
		auto decoded_size = std::size_t(0UL);
		for (auto i = 0; i < ENCODED_SIZE_BYTES; ++i) {
			decoded_size <<= BITS_IN_BYTE;
			decoded_size |= static_cast<std::size_t>(raw_size[ENCODED_SIZE_BYTES - 1 - i]);
		}
		return decoded_size;
	}
}

#endif // SIZED_PACKAGE_INFRA_HPP