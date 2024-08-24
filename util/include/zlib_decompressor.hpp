#ifndef	ZLIB_DECOMPRESSOR_HPP
#define	ZLIB_DECOMPRESSOR_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

#include "transformer.hpp"
#include "zlib.h"

namespace server_utl {
	class ZlibDecompressor: public server::Transformer<std::vector<char>(const std::vector<char>&, const std::size_t&)> {
	public:
		ZlibDecompressor() = default;
		ZlibDecompressor(const ZlibDecompressor& other) = default;
		ZlibDecompressor& operator=(const ZlibDecompressor& other) = default;

		std::vector<char> transform(const std::vector<char>& data, const std::size_t& decompressed_data_size) const override;
		server::Transformer<std::vector<char>(const std::vector<char>&, const std::size_t&)> *clone() const override;
	};

	inline std::vector<char> ZlibDecompressor::transform(const std::vector<char>& data, const std::size_t& decompressed_data_size) const {
		const auto compressed_data_size(data.size());
		std::unique_ptr<Bytef[]> compressed_data_buff(new Bytef[compressed_data_size]);
		if (!std::copy(data.begin(), data.end(), compressed_data_buff.get())) {
			throw std::runtime_error("failed to copy compressed data");
		}
		std::unique_ptr<Bytef[]> decompressed_data_buff(new Bytef[decompressed_data_size]);
		uLong actual_decompressed_data_size(decompressed_data_size);
		if (Z_OK != uncompress(decompressed_data_buff.get(), &actual_decompressed_data_size, compressed_data_buff.get(), compressed_data_size)) {
			throw std::runtime_error("failed to decompress data");
		}
		return std::vector<char>(decompressed_data_buff.get(), decompressed_data_buff.get() + actual_decompressed_data_size);
	}

	inline server::Transformer<std::vector<char>(const std::vector<char>&, const std::size_t&)> *ZlibDecompressor::clone() const {
		return new ZlibDecompressor(*this);
	}
}

#endif // ZLIB_DECOMPRESSOR_HPP